cmake_minimum_required(VERSION 3.15)
project(CPPX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mylib SHARED src/binaryTree.cpp src/node.cpp src/bst.cpp)

target_include_directories(mylib PUBLIC include)

set_target_properties(mylib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "cppx"
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests test_suite/binary_tree_test_cases.cpp)

target_link_libraries(run_tests 
    PRIVATE 
    mylib 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)