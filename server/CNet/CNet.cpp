/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <cstring>

#include "Handler/CNetHandlerWebSocket.hpp"
#include "CNetSocketInterface.hpp"

#include "CNet.hpp"

Net::CNet::CNet(const int maxCon,
                const char *ip,
                const int port) {

    listenSocket_ = CNetSocketInterface::createServerSocket(maxCon,
                                                            port,
                                                            ip);
    
    if(listenSocket_ == -1) {
        throw listenSocetInitError;
    }

    handlers_.push_back((CNetHandler *)new CNetHandlerWebSocket());

}

Net::CNet::~CNet() {
    CNetSocketInterface::closeSocket(listenSocket_);
    for(auto iter = handlers_.begin();
        iter != handlers_.end();
        ++iter) {
        delete *iter;
    }
}

void Net::CNet::close() {
    CNetSocketInterface::closeSocket(listenSocket_);
}

Net::SToken Net::CNet::acceptClient() {
    SToken result;
    result.socket_ = CNetSocketInterface::accept(listenSocket_);
    if (result.socket_ == -1) {
        std::cout<<"Cann`t accept client\n";
        return result;
    }
    std::cout << "Connect client\n";

    std::string head;
    char buffer [256];
    int sizeRead = 0;
    bool isGetHead = false;
    while(!isGetHead) {
        sizeRead = CNetSocketInterface::recv(result.socket_,
                                             buffer,
                                             256);
        if(sizeRead <= 0) {
            CNetSocketInterface::closeSocket(result.socket_);
            std::cout<<"Cann`t get head\n";
            result.socket_ = -1;
            return result;
        }
        head.insert(head.end(),
                    buffer,
                    buffer+sizeRead);
        if( head.substr(head.size() - 4, 
                        head.size()) == "\r\n\r\n" ) {
            isGetHead = true;
        }
    }

    std::cout << "Get head\n";
    for(uint i = 0; i < handlers_.size(); i++) {
        bool isSendAnswer = false;
        std::string answer = handlers_[i]->handle(head,
                                                  isSendAnswer);
        if (!answer.empty()) {
            if(isSendAnswer) {
                
                int r = CNetSocketInterface::send(result.socket_,
                                                  answer.c_str(),
                                                  answer.size());
                if(r == -1) {
                    CNetSocketInterface::closeSocket(result.socket_);
                    std::cout<<"Can`t send answer on request\n" << head;
                    result.socket_ = -1;
                    return result;
                }
                std::cout << "Send answer\n";
            }
            result.connectionType_ = i;
            return result;
        }
    }
    std::cout<<"Unknown protocol\n" << head;
    CNetSocketInterface::closeSocket(result.socket_);
    result.socket_ = -1;
    return result;
}

void Net::CNet::getData (const SToken &clientToken,
                         char **data,
                         uint &dataSize) {
    if(clientToken.socket_ == -1) {
        dataSize = 0;
        return;
    }

    uint bufferSize = 256;
    char *buffer = new char [bufferSize];
    int r = CNetSocketInterface::recv(clientToken.socket_,
                                      buffer,
                                      bufferSize);
    if(r <= 0) {
        dataSize = 0;
        return;
    }

    std::string result;
    result.insert(result.end(),
                  buffer,
                  buffer+r);
    delete [] buffer;
    bufferSize = handlers_[clientToken.connectionType_]->getSize(result);
    if(uint(r) < bufferSize) {
        buffer = new char [bufferSize];
        int r = CNetSocketInterface::recv(clientToken.socket_,
                                          buffer,
                                          bufferSize);
        if(r <= 0) {
            dataSize = 0;
            return;
        }
        result.insert(result.end(),
                      buffer,
                      buffer+r);
        delete [] buffer;
    }

    result = handlers_[clientToken.connectionType_]->unpackData(result);

    *data = new char [result.size()];
    memcpy(*data,
           result.c_str(),
           result.size());
    dataSize = result.size();
}

void Net::CNet::sendData(const SToken &clientToken,
                         const char *data,
                         const uint dataSize) {
    std::string result;
    result.insert(result.end(),
                  data,
                  data+dataSize);
    result = handlers_[clientToken.connectionType_]->packData(result);
    
    CNetSocketInterface::send(clientToken.socket_,
                              result.c_str(),
                              int ( result.size() ));
}

void Net::CNet::disconnect(SToken &clientToken) {
    Net::CNetSocketInterface::closeSocket(clientToken.socket_);
    clientToken.socket_ = -1;
}
