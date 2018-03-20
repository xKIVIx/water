/* Copyright (c) 2018, Aleksandrov Maksim */

#include <thread>

#include "gtest/gtest.h"

#include "../CNet/CNetSocketInterface.hpp"
#include "../CNet/Handler/CNetHandlerWebSocket.hpp"
#include "../CNet/CNet.hpp"

#define PORT 3000
#define ADRESS "127.0.0.1"


TEST(CNet, connection) {
    Net::CNet network(3, ADRESS, PORT);
    std::thread server (&Net::CNet::acceptClient, &network);
    sleep(1);
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
    std::string data = "hellohellohellohellohellohellohellohellohellohellokkkdooowolohellohellokkkdoohellohellohellohellohellokkkdooowolohellohellokkkdooowowowowowowqowowowowowowqwowowowowqp121237120380skjkskk2ellohellohellokkkdooowowowowowowqp121237120380skjkskk22sad322hellohellohellohellohellohellohellohellohellohellokkkdooowolohellohellokkkdoohellohellohellohellohellokkkdooowolohellohellokkkdooowowowowowowqowowowowowowqwowowowowqp121237120380skjkskk2ellohellohellokkkdooowowowowowowqp121237120380skjkskk22sad3222sad32211111111111sdxxxxxxxxxxxas221axcxc2323ds2sad32211111111111sdxxxxxxxxxxxas221axcxc2323ds";
    std::string buffer = handler.packData(data, true);
    std::cout << "-----------DATA PACK------------\n";
    for (auto iter = buffer.begin(); iter!= buffer.end(); ++iter) {
        int i = int (*iter) & 0xff;
        std::cout << std::hex << i << ' ';
    }
    std::cout << std::dec<< "\n--------------------------------\n";
    Net::SMessageHead messageHead = handler.decodMessageHead(buffer);
    std::cout << "Head size = " << messageHead.headSize_ << std::endl;
    std::cout << "Data size = " << messageHead.dataSize_ << std::endl;
    std::cout << "Type = " << messageHead.type_ << std::endl;
    std::cout << "Parametrs = " << std::hex << messageHead.parametrs_ << std::dec << std::endl;
    std::string expectResult = handler.unpackData(messageHead, buffer);
    ASSERT_STREQ(expectResult.c_str(), data.c_str()) << "Test pack data";
}

void serverWork () {
    Net::CNet network(3, ADRESS, PORT);
    Net::SToken client = network.acceptClient();
    if(client.socket_ == -1) {
        FAIL() << "accept client\n";
        return;
    }
    std::string data;
    for(short i = 0; i<10; i++) {
        bool isOk = network.getData(client, data);
        if(isOk) {
            std::cout<<"message come\n";
        } else {
            std::cout<<"message don`t come\n";
        }
        std::cout << "DATA\n[ ";
        for (auto iter = data.begin(); iter!= data.end(); ++iter) {
            int i = int (*iter) & 0xff;
            std::cout << std::hex << i << ' ';
        }
        std::cout << ']' << std::dec << std::endl;
        network.sendData(client, data);
        data.clear();
    }
    Net::CNetSocketInterface::closeSocket(client.socket_);
    network.close();
}
TEST(CNetWebSocket, connection) {
    
    std::thread server (&serverWork);
    std::string head = "GET /setConnect HTTP/1.1\r\n";
    head += "Upgrade: websocket\r\n";
    head += "Connection: Upgrade\r\n";
    head += "Host: site.com\r\n";
    head += "Sec-WebSocket-Key: Iv8io/9s+lYFgZWcXczP8Q==\r\n";
    head += "Sec-WebSocket-Version: 13\r\n";
    head += "Origin: http://site.com\r\n\r\n";

    std::string actualAnswer;
    actualAnswer += "HTTP/1.1 101 Switching Protocols\r\n";
    actualAnswer += "Upgrade: websocket\r\n";
    actualAnswer += "Connection: Upgrade\r\n";
    actualAnswer += "Sec-WebSocket-Accept: hsBlbuDTkk24srzEOTBUlZAlC2g=\r\n\r\n";

    sleep(1);
    int sock = Net::CNetSocketInterface::createClientSocket(PORT,
                                                            ADRESS);
    if(sock == -1) {
        FAIL() << "Connect error\n";
        return;
    }


    int r = Net::CNetSocketInterface::send(sock,
                                           head.c_str(),
                                           head.size());
    if(r == -1) {
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        FAIL() << "Send head error\n";
        return;
    }

    char tmp [1000];
    r = Net::CNetSocketInterface::recv(sock,
                                       tmp,
                                       1000);
    if(r == -1) {
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        FAIL() << "Get head error\n";
        return;
    }

    std::string answer;
    answer.insert(answer.end(), tmp, tmp+r);

    ASSERT_STREQ(answer.c_str(), actualAnswer.c_str()) << "Test head answer\n";
    
    Net::CNetHandlerWebSocket handler;
    std::cout << "-------------PING TEST----------\n";
    std::string pingRequest = handler.ping();
    r = Net::CNetSocketInterface::send(sock,
                                       pingRequest.c_str(),
                                       pingRequest.size());
    if(r == -1) {
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        FAIL() << "Send error\n";
        return;
    }
    char pingResponse[256];
    r = Net::CNetSocketInterface::recv(sock,
                                       pingResponse,
                                       256);
    if(r == -1) {
        Net::CNetSocketInterface::closeSocket(sock);
        server.join();
        FAIL() << "Ping error\n";
        return;
    }
    pingResponse[r] = '\0';
    ASSERT_STREQ(pingResponse, handler.pong().c_str()) << "Uncorrect ping response";


    std::cout << "-------TRANSFER DATA TEST-------\n";
    std::string testMessage = "hellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohelhellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellolohellohellohellohellohellohellohellohellohello";
    for(int i = 0; i<10; i++) {
        std::cout<<"--"<<i<<"--\n";
        std::string message = handler.packData(testMessage, true);
        r = Net::CNetSocketInterface::send(sock,
                                           message.c_str(),
                                           message.size());
        if(r == -1) {
            Net::CNetSocketInterface::closeSocket(sock);
            server.join();
            FAIL() << "Send error";
            return;
        }
        char answerMessage[1000];
        int answerSize = 1000;
        std::string comeData;
        r = Net::CNetSocketInterface::recv(sock,
                                           answerMessage,
                                           answerSize);
        comeData.insert(comeData.end(), answerMessage, answerMessage + r);
        Net::SMessageHead head = handler.decodMessageHead(comeData);
        std::cout << "Head size = " << head.headSize_ << std::endl;
        std::cout << "Data size = " << head.dataSize_ << std::endl;
        std::cout << "Type = " << head.type_ << std::endl;
        std::cout << "Parametrs = " << std::hex << head.parametrs_ << std::dec << std::endl;
        comeData = handler.unpackData(head, comeData);
        if(r == -1) {
            server.join();
            FAIL() << "Get error";
            Net::CNetSocketInterface::closeSocket(sock);
            return;
        }
        ASSERT_STREQ(comeData.c_str(), testMessage.c_str()) << "Test message :" << i;
    }
    std::cout << "--------------------------------\n";
    Net::CNetSocketInterface::closeSocket(sock);
    server.join();
}