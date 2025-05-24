# tests/CMakeLists.txt

add_subdirectory(test_framework)

set(ALL_TEST_SOURCES "")
set(COMPILE_OPTIONS -O0 -fsanitize=address -fsanitize=undefined -g)
set(LINK_OPTIONS -fsanitize=address -fsanitize=undefined)

function(add_test_executables dir)
    file(GLOB test_files "${dir}/*.cpp")

    foreach(test_file ${test_files})
        get_filename_component(test_name ${test_file} NAME_WE)

        add_executable(${test_name} ${test_file})

        target_include_directories(${test_name} PRIVATE
            ${CMAKE_SOURCE_DIR}/include/search-engine/containers/
            ${CMAKE_SOURCE_DIR}/include/search-engine/general_utilities/
            ${CMAKE_SOURCE_DIR}/include/search-engine/iterators/
            ${CMAKE_SOURCE_DIR}/include/search-engine/string/
        )

        target_link_libraries(${test_name} PRIVATE test_framework)
        target_compile_options(${test_name} PRIVATE ${COMPILE_OPTIONS})
        target_link_options(${test_name} PRIVATE ${LINK_OPTIONS})

        add_test(NAME ${test_name} COMMAND ${test_name})

        list(APPEND ALL_TEST_SOURCES ${test_file})
    endforeach()
    set(ALL_TEST_SOURCES ${ALL_TEST_SOURCES} PARENT_SCOPE)
endfunction()

add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/containers")
add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/general_utilities")
add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/iterators")
add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/string")

# All Tests binary
add_executable(all_tests ${ALL_TEST_SOURCES})
target_include_directories(all_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include/search-engine/containers/
    ${CMAKE_SOURCE_DIR}/include/search-engine/general_utilities/
    ${CMAKE_SOURCE_DIR}/include/search-engine/iterators/
    ${CMAKE_SOURCE_DIR}/include/search-engine/string/
)

target_link_libraries(all_tests PRIVATE test_framework)
target_compile_options(all_tests PRIVATE ${COMPILE_OPTIONS})
target_link_options(all_tests PRIVATE ${LINK_OPTIONS})
