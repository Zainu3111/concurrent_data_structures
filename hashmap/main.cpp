#include "coarse_grain_queue.h"
#include <iostream>
#include <deque>
#define BENCHMARK 1000000
#include <thread>

void test(coarse_grain_queue<int>& q, int Thread_num){
	for(size_t i{}; i < BENCHMARK; ++i){
		q.push(i);
	}
	for(size_t i{}; i < BENCHMARK; ++i){
		int cur;
		if(q.pop(cur)) std::cout << "By Thread " << Thread_num << ": "  << cur << std::endl;
	}
}

int main(){
	coarse_grain_queue<int> q;
	std::thread t1([&q](){
				test(q, 1);
			});
	std::thread t2([&q](){
				test(q, 2);
			});

	t1.join();
	t2.join();
	return 0;
}
