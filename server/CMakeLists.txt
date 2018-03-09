cmake_minimum_required(VERSION 3.11)

add_subdirectory(CNet)

set(LIBRES
    CNet)

project(server)
if(UTest)
    add_subdirectory(unitTests)
endif()

add_executable(server
               CReception.cpp
               main.cpp)

target_link_libraries(server ${LIBRES})
