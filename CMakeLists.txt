cmake_minimum_required(VERSION 3.15)
project(CPPX)

# Create shared library
add_library(mylib SHARED src/binaryTree.cpp src/node.cpp)

# Include directory
target_include_directories(mylib PUBLIC include)

# Set output directory
set_target_properties(mylib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "-cppx"
)
