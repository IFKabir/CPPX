cmake_minimum_required(VERSION 3.28.3)
project(CPPX)

# Create shared library
add_library(mylib SHARED src/binaryTree.cpp src/binarySearchTree.cpp)

# Include directory
target_include_directories(mylib PUBLIC include)

# Set output directory
set_target_properties(mylib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "-cppx"
)
