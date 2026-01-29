cmake_minimum_required(VERSION 3.15)
project(CPPX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Library Sources (Hardcoded to be safe)
set(LIB_SOURCES
    "src/binary_tree.cpp"
    "src/bst.cpp"
    "src/node.cpp"
)

# 2. Build Static Library
add_library(mylib STATIC ${LIB_SOURCES})
target_include_directories(mylib PUBLIC include)

# 3. Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 4. Build Test Executable (Hardcoded Sources)
add_executable(run_tests 
    "test_suite/binary_tree_test_cases.cpp"
    "test_suite/bst_test_cases.cpp"
)

target_link_libraries(run_tests 
    PRIVATE 
    mylib 
    GTest::gtest_main
)

# 5. Enable Formatting (Optional)
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCES "src/*.cpp" "include/*.h" "test_suite/*.cpp")
    add_custom_target(
        auto_format
        COMMAND ${CLANG_FORMAT} -style=file -i ${ALL_SOURCES}
    )
    add_dependencies(mylib auto_format)
endif()