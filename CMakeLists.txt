cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug )

if (UTest) 
    set(googletests_dir ${CMAKE_CURRENT_BINARY_DIR}/gtest)

    file(MAKE_DIRECTORY ${googletests_dir})

    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/gtest
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${googletests_dir})
    if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${googletests_dir})
    if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    add_subdirectory(${googletests_dir}/src
                     ${googletests_dir}/build
                     EXCLUDE_FROM_ALL)

    set (GTEST_INCLUDE_DIR
         ${googletests_dir}/src/googletest/include)
    
    set (GMOCK_INCLUDE_DIR
         ${googletests_dir}/src/googlemock/include)
    
    
endif()

add_subdirectory(server)