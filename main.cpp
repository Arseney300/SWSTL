#include "array.hpp"
#include <iostream>

int main(){
	bastion::array<int,4> a= {1,2,3,4};
	for(auto&i: a)
		std::cout << i << ' ';
}
