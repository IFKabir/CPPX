#include "../include/libraries/hello.h"
#include <iostream>

int main()
{
    your_lib::print();
    std::cout << your_lib::get_hello() << std::endl;
}