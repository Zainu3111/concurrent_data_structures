#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>

// A Baseline for what i have to build for a parallel system. 
// Another Baseline implementation just like the coarse grained 
// implementation.
template <typename T>
class seq_stack{
	private:
		std::vector<T> _stack;

	public:
		seq_stack() = default;
		~seq_stack() = default;

		// Places the value at the top into the location provided.
		inline bool pop(T& location){
			if(_stack.empty()){
				return false;
			}

			location = std::move(_stack.back());
			_stack.pop_back();
			return true;

		}

		inline void push(const T& value){
			_stack.push_back(value);
		}

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		inline void push(T&& value){
			_stack.push_back(std::move(value));
		}


		// Const cuz we do not want to modify the state of the 
		// object.
		inline bool empty() const{
			return _stack.empty();
		}

		// Similar to empty.
		inline size_t size() const{
			return _stack.size();
		}
};
#endif
