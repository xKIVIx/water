cmake_minimum_required(VERSION 3.11)

add_subdirectory(CNet)

project(server)
if(UTest)
    add_subdirectory(unitTests)
endif()
add_executable(server main.cpp)