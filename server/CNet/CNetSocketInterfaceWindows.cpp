/* Copyright (c) 2018, Aleksandrov Maksim */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


#include "CNetSocketInterface.hpp"

int Net::CNetSocketInterface::createServerSocket(const uint maxCon,
                                                 const int port,
                                                 const char *ipAdress) {
    struct addrinfo *addr = NULL, 
                     hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = CNET_DOMAIN;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int iResult = getaddrinfo(ipAdress, 
                              std::to_string(port).c_str(), 
                              &hints, 
                              (PADDRINFOA *)&addr);
    if(iResult != 0) {
        std::cout << "getaddrinfo failed: " << iResult << std::endl;
        return -1;
    }

    int result = socket(addr->ai_family,
                        addr->ai_socktype,
                        addr->ai_protocol);

    if(result == INVALID_SOCKET) {
        std::cout << "Error create socket\n";
        freeaddrinfo(addr);
        return -1;
    }
    iResult = bind(result, 
                   addr->ai_addr, 
                   (int)addr->ai_addrlen);
    if(iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addr);
        closesocket(result);
        return -1;
    }
    freeaddrinfo(addr);

    if(listen(result, maxCon) == SOCKET_ERROR) {
        std::cout << "Listen failed with error: " << WSAGetLastError();
        closesocket(result);
        return -1;
    }

    return result;
}

int Net::CNetSocketInterface::createClientSocket(const int port,
                                                 const char *ipAdress) {
    struct addrinfo *addr = NULL, 
                     hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = CNET_DOMAIN;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int iResult = getaddrinfo(ipAdress, 
                              std::to_string(port).c_str(), 
                              &hints, 
                              (PADDRINFOA *)&addr);
    if(iResult != 0) {
        std::cout << "getaddrinfo failed: " << iResult << std::endl;
        return -1;
    }

    int result = socket(addr->ai_family,
                        addr->ai_socktype,
                        addr->ai_protocol);

    if(result == INVALID_SOCKET) {
        std::cout << "Error create socket\n";
        freeaddrinfo(addr);
        return -1;
    }

    iResult = connect(result, 
                      addr->ai_addr, 
                      (int)addr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error fail connect\n";
        closesocket(result);
        result = INVALID_SOCKET;
    }

    freeaddrinfo(addr);

    return result;
}

int Net::CNetSocketInterface::accept(const int socket) {
    return ::accept(socket,
        0,
        0);
}

void Net::CNetSocketInterface::closeSocket(const int socket) {
    closesocket(socket);
}

int Net::CNetSocketInterface::send(const int socket,
    const char *data,
    const int dataSize) {
    int sendSize = 0;
    int n;
    while (sendSize < dataSize) {
        n = ::send(socket,
            data + sendSize,
            dataSize - sendSize,
            0);
        if (n < 0) {
            break;
        }
        sendSize += n;
    }

    return (n == -1 ? -1 : sendSize);
}

int Net::CNetSocketInterface::recv(const int socket,
    char *buffer,
    const int bufferSize) {
    return ::recv(socket,
        buffer,
        bufferSize,
        0);
}

void Net::CNetSocketInterface::initInterface() {
    WSADATA wsaData;

    if ( WSAStartup( MAKEWORD(2, 2), &wsaData) )
    {
        printf("Winsock not initialized !\n");
        WSACleanup();
    }
}

void Net::CNetSocketInterface::closeInterface() {
    if (WSACleanup())
        printf("WSAclean fail\n");
    else
        printf("WSAclean success\n");
}
