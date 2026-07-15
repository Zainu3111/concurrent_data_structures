#ifndef SEQ_STACK_HH
#define SEQ_STACK_HH
#include <list>
#include <utility>
#include <cstddef>
#include <shared_mutex>
#include <vector>
// For coarse-grain, I will be locking the mutex every time I 
// access the haspmap since that is the most coarse I could think 
// of. Later I plan to make fine-grained table_ and lock-free
// table_ so i could essentially benchmark the benefits of adding 
// a lot more complexity. Essentially a baseline implementation.
template <typename Key, typename Value>
class fine_grain_hashmap{
	public:
		struct ListNode{
				Key key;
				Value data;
				ListNode* next = nullptr;
				explicit ListNode() = default;
		};

		struct alignas(64) Bucket{
			mutable std::shared_mutex m;
			ListNode head;	
		};

	private:
		std::vector<Bucket> buckets;
		const size_t bucket_count;

		size_t get_index(const Key& key) const {
			auto f = std::hash<Key>{};
			return (f(key) % bucket_count);
		}
	public:
		fine_grain_hashmap(size_t num = 257)
		: buckets(num), bucket_count(num)
		{}

		~fine_grain_hashmap(){
			//TODO traverse all the buckets and delete if something
			//is in the bucket.
		}

		bool find(const Key& key, Value& val) const {
			auto index = get_index(key);
			const Bucket& bucket = buckets[index];
			std::shared_lock<std::shared_mutex> lock(bucket.m);
			ListNode* cur = bucket.head.next;
			while (cur){
				if (cur->key == key){
					val = cur->data;
					return true;
				}
				cur = cur->next;
			}
			return false;
		}

		inline bool contains(const Key& key) const {
			auto index = get_index(key);
			const Bucket& bucket = buckets[index];
			std::shared_lock<std::shared_mutex> lock(bucket.m);
			ListNode* cur = bucket.head.next;
			while (cur){
				if (cur->key == key){
					return true;
				}
				cur = cur->next;
			}
			return false;
		}

		inline bool erase(const Key& key){
		}

		inline bool insert(Key&& key, Value&& val){
		}

		inline void clear(){
		}

		inline bool empty() const{
		}

		inline size_t size() const{
		}
};
#endif
