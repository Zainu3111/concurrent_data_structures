#ifndef FINE_GRAIN_QUEUE_HH
#define FINE_GRAIN_QUEUE_HH


// Fine-Grained Queue
//
// Synchronization:
//   - head_mutex protects head.
//   - tail_mutex protects tail.
//
// Complexity:
//   push()    : O(1)
//   try_pop() : O(1)
//
// Concurrency:
//   - Concurrent push() and pop() are allowed.
//   - Multiple pushes serialize on tail_mutex.
//   - Multiple pops serialize on head_mutex.
//
// Design:
//   Uses a moving dummy node to simplify the empty queue case.


#include <cstddef>
#include <mutex>
#include <utility>

// Since a queue has two points of access, there is relatively more 
// oppertunity for fine grain locking. Having 2 locks, one for head
// and another for the tail allows 2 threads to concurrently access 
// the queue. Unfortunately, we have to lock the tail while popping 
// for a small section in order to make sure we that we have enough 
// elements in the queue. Hopefully, we see some benefit of this fine 
// grain locking when benchmarking. However, I speculate that the 
// added extra work of loading linked lists, we might not see a huge
// difference in practice.
template <typename T>
class fine_grain_queue{
	struct ListNode{
			T value;
			ListNode* next = nullptr;
			explicit ListNode() = default;
			explicit ListNode(T&& val)
				: value(std::move(val)) {}
			explicit ListNode(const T& val)
				: value(val) {}
	};

	private:
		mutable std::mutex tail_lock;
		mutable std::mutex head_lock;
		ListNode* head = nullptr;
		ListNode* tail = nullptr;

	public:
		fine_grain_queue(){
			auto* dummy = new ListNode();
			head = dummy;
			tail = dummy;
		}

		// Make sure that we do not end up with a leak.
		~fine_grain_queue(){
			std::scoped_lock guard(head_lock, tail_lock); 
			while(head){
				auto temp = head;
				head = head->next;
				delete temp;
			}
		}

		inline ListNode* get_tail(){
			std::lock_guard guard(tail_lock);
			return tail;
		}

		// The head always point to a dummy node, we move the head,
		// and delete the old head and move the new head value into
		// the location, essentially making the new head the dummy
		// node.
		inline bool try_pop(T& location){
			
			std::lock_guard guard(head_lock);
			if(head == get_tail()){
				return false;
			}
			auto temp = head;
			head = head->next;
			location = std::move(head->value);
			delete temp;
			return true;
		}

		inline void push(const T& value){
			auto* temp = new ListNode(value);
			std::lock_guard<std::mutex> guard(tail_lock);
			tail->next = temp;
			tail = temp;
		}

		inline void push(T&& value){
			auto* temp = new ListNode(std::move(value));
			std::lock_guard<std::mutex> guard(tail_lock);
			tail->next = temp;
			tail = temp;
		}

		// Using scoped_locks in order to lock both of them at the 
		// same time in order to avoid a dead-lock.
		inline bool empty() const{
			std::scoped_lock guard(head_lock, tail_lock);
			return head == tail;
		}
};
#endif
