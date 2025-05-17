# tests/CMakeLists.txt

add_subdirectory(test_framework)

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
        target_compile_options(${test_name} PRIVATE -Wall -Wextra -g)

        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endfunction()

add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/containers")
add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/general_utilities")
add_test_executables("${CMAKE_CURRENT_SOURCE_DIR}/string")
