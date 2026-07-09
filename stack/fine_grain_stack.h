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
class fine_grain_stack{
	struct ListNode{
			T value;
			ListNode* next = nullptr;
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
				delete temp;
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
};
#endif
