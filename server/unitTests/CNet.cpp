/* Copyright (c) 2018, Aleksandrov Maksim */

#include <thread>

#include "gtest/gtest.h"

#include "../CNet/CNetSocketInterface.hpp"
#include "../CNet/Handler/CNetHandlerWebSocket.hpp"
#include "../CNet/CNet.hpp"

#define PORT 3000
#define ADRESS "127.0.0.1"


TEST(CNet, connection) {
    Net::CNet network;
    std::thread server (&Net::CNet::acceptClient, &network);
    
    int sock = Net::CNetSocketInterface::createClientSocket(PORT,
                                                            ADRESS);

    if(sock == -1){
        FAIL();
    }
    Net::CNetSocketInterface::closeSocket(sock);
    server.join();
	SUCCEED();
}

TEST(CNetWebSocket, package) {
    Net::CNetHandlerWebSocket handler;
    std::string data = "hellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohelhellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellolohellohellohellohellohellohellohellohellohello";
    std::string actualResult = "\x80\x83\x60";
    actualResult += data;
    std::string expectResult = handler.packData(data);
    ASSERT_STREQ(expectResult.c_str(), actualResult.c_str()) << "Test pack data";
    ASSERT_STREQ(handler.unpackData(expectResult).c_str(), data.c_str()) << "Test unpack data";
}
void serverWork () {
    Net::CNet network;
    Net::SToken client = network.acceptClient();
    if(client.socket_ == -1) {
        FAIL() << "accept client\n";
        return;
    }
    char * data = nullptr;
    uint sizeData = 0;
    for(short i = 0; i<10; i++) {
        network.getData(client, &data, sizeData);
        network.sendData(client, data, sizeData);
        if(sizeData != 0) {
            delete [] data;
        }
    }
    Net::CNetSocketInterface::closeSocket(client.socket_);
}
TEST(CNetWebSocket, connection) {
    
    std::thread server (&serverWork);

    std::string head = "GET /setConnect HTTP/1.1\r\n";
    head += "Upgrade: WebSocket\r\n";
    head += "Connection: Upgrade\r\n";
    head += "Host: site.com\r\n";
    head += "Origin: http://site.com\r\n\r\n";

    std::string actualAnswer;
    actualAnswer += "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
    actualAnswer += "Upgrade: WebSocket\r\n";
    actualAnswer += "Connection: Upgrade\r\n";
    actualAnswer += "WebSocket-Origin: http://localhost:6000\r\n";
    actualAnswer += "WebSocket-Location: ws://localhost:6000\r\n\r\n";

    sleep(3);
    int sock = Net::CNetSocketInterface::createClientSocket(PORT,
                                                            ADRESS);
    if(sock == -1) {
        FAIL() << "Connect error";
        return;
    }


    int r = Net::CNetSocketInterface::send(sock,
                                           head.c_str(),
                                           head.size());
    if(r == -1) {
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        FAIL() << "Send head error";
        return;
    }

    char tmp [1000];
    r = Net::CNetSocketInterface::recv(sock,
                                       tmp,
                                       1000);
    if(r == -1) {
        FAIL() << "Get head error";
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        return;
    }

    std::string answer;
    answer.insert(answer.end(), tmp, tmp+r);

    ASSERT_STREQ(answer.c_str(), actualAnswer.c_str()) << "Test head answer";

    std::string testMessage = "\x80\x83\x60hellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohelhellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellolohellohellohellohellohellohellohellohellohello";
    for(int i = 0; i<10; i++) {
        r = Net::CNetSocketInterface::send(sock,
                                           testMessage.c_str(),
                                           testMessage.size());
        if(r == -1) {
            FAIL() << "Send error";
            Net::CNetSocketInterface::closeSocket(sock);
            server.join();
            return;
        }
        char answerMessage[484];
        int answerSize = 484;
        r = Net::CNetSocketInterface::recv(sock,
                                           answerMessage,
                                           answerSize);
        if(r == -1) {
            FAIL() << "Get error";
            Net::CNetSocketInterface::closeSocket(sock);
            server.join();
            return;
        }
        ASSERT_STREQ(answerMessage, testMessage.c_str()) << "Test message :" << i;
    }
    Net::CNetSocketInterface::closeSocket(sock);
    server.join();
}