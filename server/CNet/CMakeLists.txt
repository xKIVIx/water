cmake_minimum_required(VERSION 3.11)

set(SOURCE
    CNet.cpp
    Handler/CNetHandlerWebSocket.cpp)


if(${OS} STREQUAL "linux")
    set(SOURCE
        ${SOURCE}
        CNetSocketInterfaceLinux.cpp)
endif()


add_library( CNet STATIC ${SOURCE} )