cmake_minimum_required(VERSION 3.14)
project(CPPX VERSION 1.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.tpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")

# FIX 1: Make this an INTERFACE library since it relies on templates
add_library(${PROJECT_NAME}_lib INTERFACE)
target_include_directories(${PROJECT_NAME}_lib INTERFACE include src)

# --- Auto-Format ---
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
        "src/*.cpp" "src/*.tpp" "src/*.h" "src/*.hpp"
        "include/*.h" "include/*.hpp" "test_suite/*.cpp"
    )
    add_custom_target(format_code
        COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_SOURCE_FILES}
        COMMENT "Auto-formatting code..."
    )
endif()

# --- Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB_RECURSE TEST_FILES "test_suite/*.cpp")
add_executable(unit_tests ${TEST_FILES})
target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME}_lib GTest::gtest_main)
target_include_directories(unit_tests PRIVATE include)

# FIX 2: Attach the formatter directly to the executable target
if(TARGET format_code)
    add_dependencies(unit_tests format_code)
endif()

include(GoogleTest)
gtest_discover_tests(unit_tests)

# --- Doxygen Awesome Theme ---
FetchContent_Declare(
    doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(doxygen-awesome-css)
set(AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})

# --- Doxygen Generation ---
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    if(WIN32)
        add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Awesome API docs on Windows..."
            VERBATIM)
    else()
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Awesome API docs on Ubuntu..."
            VERBATIM)
    endif()
else()
    message(WARNING "Doxygen not found!")
endif()