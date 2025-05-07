# tests/test_framework/CMakeLists.txt

add_library(test_framework STATIC
    test_framework.cpp
)

target_include_directories(test_framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(test_framework PRIVATE -g)
