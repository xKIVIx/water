/* Copyright (c) 2018, Aleksandrov Maksim */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include "gtest/gtest.h"
#include "../CNet/CNet.hpp"

TEST(CNet, connection)
{
    Net::CNet network;
    std::thread server (&Net::CNet::listen, &network);
    
    int sock = socket(CNET_DOMAIN,
                      SOCK_STREAM,
                      0);

    struct sockaddr_in addr;

    addr.sin_family = CNET_DOMAIN;
    addr.sin_port = htons(CNET_PORT); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        FAIL();
    }
    close(sock);
    server.join();
	SUCCEED();
}