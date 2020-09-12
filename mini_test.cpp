#include <iostream>
#include "vector.hpp"
int main(){
    bastion::vector<int> vec;
    for(int i =0;i< 20; i++)
        vec.push_back(i);
    for(auto&i:vec)
        std::cout << vec.at(i) << " ";
    std::cout << std::endl;
    bastion::gr_vector<int> vec2;
     for(int i =0;i< 30; i++)
        vec2.push_back(i);
    for(auto&i:vec2)
        std::cout << vec.at(i) << " ";
}