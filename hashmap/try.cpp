#include <iostream>
#include <functional>
#include <string>
int main(){
	std::string s1 = "My name is Zain";
	std::string s2 = "My name is Zain";
	std::hash<std::string> f;
	std::cout << f(s1) % 15 << std::endl;
	std::cout << f(s2) % 15 << std::endl;
	return 0;
}
