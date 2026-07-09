#include <cstddef>
template <typename T>
size_t seq_stack<T>::size() const {
	return _stack.size();
}

template <typename T>
bool seq_stack<T>::empty() const {
	return _stack.empty();
}

template <typename T>
void seq_stack<T>::push(T&& value){
	_stack.push_back(std::move(value));
}

template <typename T>
void seq_stack<T>::push(const T& value){
	_stack.push_back(value);
}

template <typename T>
bool seq_stack<T>::pop(T& location){
	
	if(empty()){
		return false;
	}

	location = std::move(_stack.back());
	_stack.pop_back();
	return true;
}

