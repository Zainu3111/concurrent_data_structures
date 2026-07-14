#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <unordered_map>
#include <cstddef>
#include <mutex>

// For coarse-grain, I will be locking the mutex every time I 
// access the haspmap since that is the most coarse I could think 
// of. Later I plan to make fine-grained table_ and lock-free
// table_ so i could essentially benchmark the benefits of adding 
// a lot more complexity. Essentially a baseline implementation.
template <typename Key, typename Value>
class coarse_grain_hashmap{
	private:
		mutable std::mutex lock_;
		std::unordered_map<Key, Value> table_;

	public:
		coarse_grain_hashmap() = default;

		inline bool find(const Key& key, Value& val) const {
			std::lock_guard guard(lock_);

			auto it = table_.find(key);
			if(it != table_.end()){
				val = it->second;
				return true;
			}
			return false;
		}

		inline bool contains(const Key& key) const {
			std::lock_guard guard(lock_);

			return table_.contains(key);
		}

		inline bool erase(const Key& key){
			std::lock_guard guard(lock_);

			if(table_.contains(key)){
				table_.erase(key);
				return true;
			}
			return false;
		}

		inline bool insert(Key&& key, Value&& val){
			std::lock_guard guard(lock_);

		return table_.emplace(
				std::forward<Key>(key),
				std::forward<Value>(val)
				).second;	
		}

		inline void clear(){
			std::lock_guard guard(lock_);
			table_.clear();
		}

		inline bool empty() const{
			std::lock_guard<std::mutex> guard(lock_);
			return table_.empty();
		}

		inline size_t size() const{
			std::lock_guard<std::mutex> guard(lock_);
			return table_.size();
		}
};
#endif
