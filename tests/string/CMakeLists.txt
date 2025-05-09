# tests/containers/CMakeLists.txt

add_executable(test_string test_string.cpp)

target_include_directories(test_string PRIVATE
    ${CMAKE_SOURCE_DIR}/include/search-engine/containers/
    ${CMAKE_SOURCE_DIR}/include/search-engine/string/
)

target_link_libraries(test_string PRIVATE
    test_framework
)

target_compile_options(test_string PRIVATE -Wall -Wextra -g)

add_test(NAME test_string COMMAND test_string)
