#include <iostream>
#include "string.hpp"
int main(int argc, char**argv){
	bastion::string str;
	str = "hello";
	str+=" world!";
	std::cout << str;
}
