cmake_minimum_required(VERSION 3.9)

add_subdirectory(CNet)

set(LIBRES
    CNet)

project(server)
if(UTest)
    add_subdirectory(unitTests)
endif()

add_executable(server
               CReception.cpp
               CServer.cpp
               main.cpp)

target_link_libraries(server ${LIBRES})
