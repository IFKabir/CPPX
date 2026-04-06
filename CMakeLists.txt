cmake_minimum_required(VERSION 3.14)
project(CPPX VERSION 3.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(${PROJECT_NAME}_lib INTERFACE)
target_include_directories(${PROJECT_NAME}_lib INTERFACE include src)

option(CPPX_BUILD_TESTS "Build the GoogleTest suite" ON)
option(CPPX_BUILD_DOCS "Build the Doxygen documentation" ON)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
        "src/*.cpp" "src/*.tpp" "src/*.h" "src/*.hpp"
        "include/*.h" "include/*.hpp" "test_suite/*.cpp"
        "benchmarks/*.cpp"
    )
    add_custom_target(format_code
        COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_SOURCE_FILES}
        COMMENT "Auto-formatting code..."
    )
endif()

# --- Testing & Benchmarks Section ---
if(CPPX_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        v1.14.0 
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "test_suite/*.cpp")
    add_executable(unit_tests ${TEST_FILES})
    target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME}_lib GTest::gtest_main)
    target_include_directories(unit_tests PRIVATE include)

    if(TARGET format_code)
        add_dependencies(unit_tests format_code)
    endif()

    include(GoogleTest)
    gtest_discover_tests(unit_tests)

    add_executable(benchmark benchmarks/benchmark_main.cpp)
    target_link_libraries(benchmark PRIVATE ${PROJECT_NAME}_lib)
    target_include_directories(benchmark PRIVATE include)
    target_compile_options(benchmark PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -march=native>
        $<$<CXX_COMPILER_ID:MSVC>:/O2>
    )

    if(TARGET format_code)
        add_dependencies(benchmark format_code)
    endif()

    add_custom_target(run_benchmark
        COMMAND $<TARGET_FILE:benchmark> ${CMAKE_SOURCE_DIR}
        DEPENDS benchmark
        COMMENT "Running CPPX benchmark suite..."
        VERBATIM
    )
endif()

# --- Documentation Section ---
if(CPPX_BUILD_DOCS)
    include(FetchContent)
    FetchContent_Declare(
        doxygen-awesome-css
        URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(doxygen-awesome-css)
    set(AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})

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
endif()

include(GNUInstallDirs)

install(DIRECTORY include/ src/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppx
        FILES_MATCHING PATTERN "*.h" PATTERN "*.tpp")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "cppx")
set(CPACK_PACKAGE_CONTACT "IFKabir <ifk.ishanfalak@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High-performance C++23 extended data structures library with arena allocation.")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all") 
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
include(CPack)