cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE} )

find_package( Threads )
link_libraries( ${CMAKE_THREAD_LIBS_INIT} )

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
	set (CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
endif()


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


find_package( OpenCL )
if( NOT ${OpenCL_FOUND} )
	set(OpenCL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/OpenCL/Nvidia/Win32)
	set(OpenCL_INCLUDE_DIRS ${OpenCL_PATH}/include)
	set(OpenCL_LIBRARIES ${OpenCL_PATH}/lib/OpenCl.lib)
endif()

set(settingManager_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/settingsManager/include)
set(waterCompute_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/waterCompute/include)

add_subdirectory(settingsManager)
add_subdirectory(waterCompute)
add_subdirectory(server)