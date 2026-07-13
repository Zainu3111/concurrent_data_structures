#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>
#include <mutex>
#include <utility>

// Since a stack only has one point to access, there is relatively
// little oppertunity for fine grain locks. The only improvement
// i can see over the coarse grain stack is that we are making the 
// nodes before locking, essentially reducing the critical section.
// This could improve performance in case the object is bigger 
// and more complex. 
template <typename T>
class fine_grain_queue{
	struct ListNode{
			T value;
			ListNode* next = nullptr;
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
			auto dummy = new ListNode(0);
			head = &dummy;
			tail = &dummy;
		}

		~fine_grain_queue(){
			std::scoped_lock<std::mutex, std::mutex> guard(head_lock, tail_lock); 
			while(head){
				auto temp = head;
				head = head->next;
				delete temp;
			}
		}

		inline bool get_tail(){
			std::lock_guard guard(tail_lock);
			return tail;
		}

		inline bool try_pop(T& location){
			
			std::lock_guard guard(head_lock);
			if(head == tail){
				return false;
			}
			auto temp = head->next;
			head = head->next;
			location = std::move(head->val);
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


		inline bool empty() const{
			std::lock_guard<std::mutex> guard(head_lock);
			return head->next == nullptr;
		}
};
#endif
