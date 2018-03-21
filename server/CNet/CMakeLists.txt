cmake_minimum_required(VERSION 3.9)

set(SOURCE
    CNet.cpp
    Handler/CNetHandlerWebSocket.cpp)


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SOURCE
        ${SOURCE}
        CNetSocketInterfaceLinux.cpp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(SOURCE
        ${SOURCE}
        CNetSocketInterfaceWindows.cpp)
endif()


add_library( CNet STATIC ${SOURCE} )