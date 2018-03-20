/* Copyright (c) 2018, Aleksandrov Maksim */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "CNetSocketInterface.hpp"

int Net::CNetSocketInterface::createServerSocket(const uint maxCon,
                                                 const int port,
                                                 const char *ipAdress) {
    int result = ::socket(CNET_DOMAIN, 
                          SOCK_STREAM, 
                          0);

    if (result == -1) {
        std::cout << "Socket create error!\n";
        return -1;
    }

    struct sockaddr_in addr;

    addr.sin_family = CNET_DOMAIN;
    addr.sin_port = htons(port);
    if(inet_aton(ipAdress,&addr.sin_addr) == 0) {
        std::cout << "IP adress error: " << ipAdress << std::endl;
    }
    int r = ::bind(result, 
                   (struct sockaddr *)&addr, 
                   sizeof(addr));

    if(r < 0) {
        std::cout << "Listen socket bind error!\n";
        return -1;
    }

    ::listen(result, maxCon);

    return result;
}

int Net::CNetSocketInterface::createClientSocket(const int port,
                                                 const char *ipAdress) {
    struct sockaddr_in addr;

    int result = socket(CNET_DOMAIN,
                        SOCK_STREAM, 
                        0);
    if (result == -1) {
        std::cout << "Socket create error!";
        return -1;
    }

    addr.sin_family = CNET_DOMAIN;
    addr.sin_port = htons(port);
    if(inet_aton(ipAdress,&addr.sin_addr) == 0) {
        std::cout << "IP adress error: " << ipAdress << std::endl;
    }
    if( ::connect(result, 
                  (struct sockaddr *)&addr, 
                  sizeof(addr)) < 0) {
        std::cout << "Connect error!\n";
        closeSocket(result);
        return -1;
    }
    return result;
}

int Net::CNetSocketInterface::accept(const int socket) {
    return ::accept(socket, 
                    0, 
                    0);
}

void Net::CNetSocketInterface::closeSocket(const int socket) {
    ::close(socket);
}

int Net::CNetSocketInterface::send(const int socket,
                                   const char *data,
                                   const int dataSize) {
    int sendSize = 0;
    int n;
    while(sendSize < dataSize) {
        n = ::send(socket,
                   data + sendSize,
                   dataSize - sendSize,
                   0);
        if(n < 0) { 
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

