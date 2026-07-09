#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>
#include <mutex>

// For coarse-grain, I will be locking the mutex every time I 
// access the stack since that is the most coarse I could think 
// of. Later I plan to make fine-grained stack and lock-free
// stack so i could essentially benchmark the benefits of adding 
// a lot more complexity. Essentially a baseline implementation.
template <typename T>
class fine_grain_stack{
	class ListNode{
		private:
			T value;
			ListNode* next = nullptr;

		public:
			explicit ListNode(T&& val)
				: value(std::move(val)) {}
			explicit ListNode(const T& val)
				: value(val) {}
	};

	private:
		mutable std::mutex _lock;
		ListNode* head = nullptr;

	public:
		fine_grain_stack() = default;
		~fine_grain_stack(){
			std::lock_guard<std::mutex> guard(_lock);
			while(head){
				auto temp = head;
				head = head->next;
				delete head;
			}
		}

		// Places the value at the top into the location provided.
		inline bool pop(T& location){
			// Using std::lock_guard so I do not have to free the
			// lock everytime and helps in case i forget to unlock.
			std::lock_guard<std::mutex> guard(_lock);
			if(head == nullptr){
				return false;
			}

			location = std::move(head->value);
			auto temp = head;
			head = head->next;
			delete temp;
			return true;
		}

		inline void push(const T& value){
			auto* temp = new ListNode(value);
			std::lock_guard<std::mutex> guard(_lock);
			temp->next = head;
			head = temp;
		}

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		inline void push(T&& value){
			auto* temp = new ListNode(std::move(value));
			std::lock_guard<std::mutex> guard(_lock);
			temp->next = head;
			head = temp;
		}


		// Const cuz we do not want to modify the state of the 
		// object.
		inline bool empty() const{
			std::lock_guard<std::mutex> guard(_lock);
			return head == nullptr;
		}
/*
		// Similar to empty.
		inline size_t size() const{
			std::lock_guard<std::mutex> guard(_lock);
			return _stack.size();
		}
*/
};
#endif
