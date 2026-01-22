cmake_minimum_required(VERSION 3.15)
project(CPPX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(mylib SHARED ${LIB_SOURCES})

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

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "test_suite/*.cpp")

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests 
    PRIVATE 
    mylib 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)