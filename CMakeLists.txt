cmake_minimum_required(VERSION 3.12)

project(
    ArgParser
    VERSION 1.0
    DESCRIPTION "A C++ library for parsing command-line arguments. This library is designed to simplify the process of handling command-line options in your C++ programs by supporting various argument types, automatic help generation, and positional arguments"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)
