cmake_minimum_required(VERSION 3.14)
project(CPPX VERSION 1.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.tpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")

add_library(${PROJECT_NAME}_lib ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC 
    include 
    src
)

set_target_properties(${PROJECT_NAME}_lib PROPERTIES LINKER_LANGUAGE CXX)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
        "src/*.cpp" "src/*.tpp" "src/*.h" "src/*.hpp"
        "include/*.h" "include/*.hpp"
        "test_suite/*.cpp"
    )
    add_custom_target(
        format_code
        COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_SOURCE_FILES}
        COMMENT "Auto-formatting code with clang-format..."
    )
    add_dependencies(${PROJECT_NAME}_lib format_code)
else()
    message(WARNING "clang-format not found! Code will not be auto-formatted.")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TEST_FILES "test_suite/*.cpp")

add_executable(unit_tests ${TEST_FILES})
target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME}_lib GTest::gtest_main)
target_include_directories(unit_tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(unit_tests)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else()
    message(WARNING "Doxygen not found! Documentation will not be generated.")
endif()
