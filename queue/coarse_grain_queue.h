#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>
#include <mutex>

// For coarse-grain, I will be locking the mutex every time I 
// access the queue since that is the most coarse I could think 
// of. Later I plan to make fine-grained stack and lock-free
// queue so i could essentially benchmark the benefits of adding 
// a lot more complexity. Essentially a baseline implementation.
template <typename T>
class coarse_grain_queue{
	private:
		mutable std::mutex _lock;
		std::vector<T> _stack;

	public:
		coarse_grain_stack() = default;
		~coarse_grain_stack() = default;

		// Places the value at the top into the location provided.
		inline bool pop(T& location){
			// Using std::lock_guard so I do not have to free the
			// lock everytime and helps in case i forget to unlock.
			std::lock_guard<std::mutex> guard(_lock);
			if(_stack.empty()){
				return false;
			}

			location = std::move(_stack.back());
			_stack.pop_back();
			return true;

		}

		inline void push(const T& value){
			std::lock_guard<std::mutex> guard(_lock);
			_stack.push_back(value);
		}

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		inline void push(T&& value){
			std::lock_guard<std::mutex> guard(_lock);
			_stack.push_back(std::move(value));
		}


		// Const cuz we do not want to modify the state of the 
		// object.
		inline bool empty() const{
			std::lock_guard<std::mutex> guard(_lock);
			return _stack.empty();
		}

		// Similar to empty.
		inline size_t size() const{
			std::lock_guard<std::mutex> guard(_lock);
			return _stack.size();
		}
};
#endif
