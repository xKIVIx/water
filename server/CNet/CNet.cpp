/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <cstring>

#include "Handler/CNetHandlerWebSocket.hpp"
#include "CNetSocketInterface.hpp"

#include "CNet.hpp"

Net::CNet::CNet(const int maxCon,
                const std::string &ip,
                const int port) {

    listenSocket_ = CNetSocketInterface::createServerSocket(maxCon,
                                                            port,
                                                            ip.c_str());
    
    if(listenSocket_ == -1) {
        throw listenSocetInitError;
    }
    ip_ = ip;
    port_ = port;
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
    int sock = CNetSocketInterface::createClientSocket(port_,
                                                       ip_.c_str());
    CNetSocketInterface::send(sock,"\r\n\r\n",5);
    CNetSocketInterface::closeSocket(sock);
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

    std::cout << "Get head\n" << head;
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
                std::cout << "Send answer\n" << answer;
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

bool Net::CNet::getData (const SToken &clientToken,
                         std::string &data) {
    if(clientToken.socket_ == -1) {
        return false;
    }

    // get message
    while(true) {
        std::string packData;
        uint headSize = handlers_[clientToken.connectionType_]
                            ->getMessageHeadMaxSize();
        char * buffer = new char [headSize];

        int r = CNetSocketInterface::recv(clientToken.socket_,
                                        buffer,
                                        int(headSize));
        if(r <= 0) {
            return false;
        }
        packData.reserve(r);
        packData.insert(packData.end(),
                        buffer,
                        buffer + r);
        delete [] buffer;
        SMessageHead messageHead = handlers_[clientToken.connectionType_]
                                    ->decodMessageHead(packData);
        if(messageHead.type_ == MessageType::PING) {
            std::string pong = handlers_[clientToken.connectionType_]
                                ->pong();
            r = CNetSocketInterface::send(clientToken.socket_,
                                        pong.c_str(),
                                        int(pong.size()));
            if(r <= 0) {
                std::cout << "Pong send error\n";
                return false;
            }
            continue;
        } else if(messageHead.type_ == MessageType::DISCONNECT) {
            std::cout << "Disconnect\n";
            return false;
        } else if((messageHead.type_ == MessageType::DATA)||
                  (messageHead.type_ == MessageType::DATA_FRAGMENT)) {
            uint messageSize = messageHead.dataSize_ + 
                               messageHead.headSize_;
            if(messageSize > packData.size()) {
                uint lastPartSize = messageSize - packData.size();
                buffer = new char [lastPartSize];
                r = CNetSocketInterface::recv(clientToken.socket_,
                                              buffer,
                                              int(lastPartSize));
                packData.reserve(messageSize);
                packData.insert(packData.end(),
                                buffer,
                                buffer + r);
                delete [] buffer;
            }
            data += handlers_[clientToken.connectionType_]
                        ->unpackData(messageHead, packData);
            if(messageHead.type_ == MessageType::DATA) {
                return true;
            } else {
                continue;
            }
        } else if(messageHead.type_ == MessageType::PONG) {
            return true;
        } else {
            std::cout << "Unknown message\n";
            return false;
        }     
    }
}

void Net::CNet::sendData(const SToken &clientToken,
                         const std::string &data) {
    if((clientToken.socket_ <= 0)||
       (data.empty())) {
        return;
    }
    std::string result = handlers_[clientToken.connectionType_]
                            ->packData(data, true);
    CNetSocketInterface::send(clientToken.socket_,
                              result.c_str(),
                              int ( result.size() ));
}

void Net::CNet::disconnect(SToken &clientToken,
                           DisconnectReason reason) {
    std::string disconnectMes = handlers_[clientToken.connectionType_]
                            ->disconect(reason);
    CNetSocketInterface::send(clientToken.socket_,
                              disconnectMes.c_str(),
                              disconnectMes.size());
    Net::CNetSocketInterface::closeSocket(clientToken.socket_);
    clientToken.socket_ = -1;
}
