# tests/containers/CMakeLists.txt

add_executable(test_vector test_vector.cpp)

target_include_directories(test_vector PRIVATE
    ${CMAKE_SOURCE_DIR}/include/search-engine/containers
)

target_link_libraries(test_vector PRIVATE
    test_framework
)

target_compile_options(test_vector PRIVATE -Wall -Wextra -g)

add_test(NAME test_vector COMMAND test_vector)
