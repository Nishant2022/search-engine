cmake_minimum_required(VERSION 4.0.1)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -pedantic -pthread")

enable_testing()
add_subdirectory(tests)
