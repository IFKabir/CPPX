#include "../include/libraries/hello.h"
#include <iostream>

namespace your_lib
{
    void print()
    {
        std::cout << "How are you doing?" << std::endl;
    }
    std::string get_hello()
    {
        return "Hello, World";
    }
}