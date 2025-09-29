cmake_minimum_required(VERSION 3.10)
project(your_cpp_library)

add_library(your_lib src/hello.cpp)
target_include_directories(your_lib PRIVATE include)

add_executable(example examples/example.cpp)
target_link_libraries(example your_lib)
