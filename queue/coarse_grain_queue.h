#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <deque>
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
		std::deque<T> _queue;

	public:
		coarse_grain_queue() = default;
		~coarse_grain_queue() = default;

		// Places the value at the top into the location provided.
		inline bool try_pop(T& location){
			// Using std::lock_guard so I do not have to free the
			// lock everytime and helps in case i forget to unlock.
			std::lock_guard<std::mutex> guard(_lock);
			if(_queue.empty()){
				return false;
			}

			location = std::move(_queue.front());
			_queue.pop_front();
			return true;

		}

		inline void push(const T& value){
			std::lock_guard<std::mutex> guard(_lock);
			_queue.push_back(value);
		}

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		inline void push(T&& value){
			std::lock_guard<std::mutex> guard(_lock);
			_queue.push_back(std::move(value));
		}


		// Const cuz we do not want to modify the state of the 
		// object.
		inline bool empty() const{
			std::lock_guard<std::mutex> guard(_lock);
			return _queue.empty();
		}

		// Similar to empty.
		inline size_t size() const{
			std::lock_guard<std::mutex> guard(_lock);
			return _queue.size();
		}
};
#endif
