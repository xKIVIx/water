cmake_minimum_required(VERSION 3.11)

if(${OS} STREQUAL "linux")
    set(SOURCE
        CNetLinux.cpp)
endif()


add_library( CNet STATIC ${SOURCE} )