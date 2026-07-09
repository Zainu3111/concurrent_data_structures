#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <vector>
#include <cstddef>

template <typename T>
class seq_stack{
	private:
		std::vector<T> _stack;

	public:
		seq_stack() = default;
		~seq_stack() = default;

		// Places the value at the top into the location provided.
		bool pop(T&);

		void push(const T&);

		// Universal refrence -> allows for lvalues and rvalues
		// -> std::forward
		void push(T&&);

		// Const cuz we do not want to modify the state of the 
		// object.
		bool empty() const;

		// Similar to empty.
		size_t size() const;
};

#include "./seq_stack.tpp"
#endif
