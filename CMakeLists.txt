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

find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    message(STATUS "Doxygen found. You can build docs using 'cmake --build build --target doc'")
else()
    message(WARNING "Doxygen not found. Documentation target will not be available.")
endif()

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCES "src/*.cpp" "include/*.h" "test_suite/*.cpp")

    add_custom_target(
        auto_format
        COMMAND ${CLANG_FORMAT} -style=file -i ${ALL_SOURCES}
        COMMENT "🎨 Auto-formatting code with clang-format..."
    )

    add_dependencies(mylib auto_format)
else()
    message(WARNING "clang-format not found. Auto-formatting is disabled.")
endif()

add_custom_command(
    TARGET run_tests
    POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "🧪 Running tests automatically after build..."
)