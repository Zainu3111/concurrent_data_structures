#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>

// A Baseline for what i have to build for a parallel system. 
// Another Baseline implementation just like the coarse grained 
// implementation.
template <typename T>
class seq_queue{
	private:
		std::vector<T> _queue;

	public:
		seq_queue() = default;
		~seq_queue() = default;

		// Places the value at the top into the location provided.
		inline bool pop(T& location){
			if(_queue.empty()){
				return false;
			}

			location = std::move(_queue.front());
			_queue.pop_front();
			return true;

		}

		inline void push(const T& value){
			_queue.push_back(value);
		}

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		inline void push(T&& value){
			_queue.push_back(std::move(value));
		}


		// Const cuz we do not want to modify the state of the 
		// object.
		inline bool empty() const{
			return _queue.empty();
		}

		// Similar to empty.
		inline size_t size() const{
			return _queue.size();
		}
};
#endif
