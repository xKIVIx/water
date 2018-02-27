/* Copyright (c) 2018, Aleksandrov Maksim */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "CNet.hpp"

#define INVALID_SOCET(x) x<0

Net::CNet::CNet() {
    listenSocket_ = socket(CNET_DOMAIN,
                           SOCK_STREAM,
                           0);

    if ( !INVALID_SOCET(listenSocket_) ) {
        std::cout << "Listen socket init error!";
        throw Exception::listenSocetInitError;
    }

    struct sockaddr_in addr;

    addr.sin_family = CNET_DOMAIN;
    addr.sin_port = htons(CNET_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(listenSocket_, 
                 (struct sockaddr *)&addr, 
                 sizeof(addr));
    if(r < 0) {
        std::cout << "Listen socket bind error!";
        throw Exception::listenSocetBindError;
    }

    ::listen(listenSocket_, 1);
    std::cout << "Network success init\n";
}

int Net::CNet::listen() {
    std::cout << "Wait connection\n";
    return accept(listenSocket_, 
                  0, 
                  0);
}