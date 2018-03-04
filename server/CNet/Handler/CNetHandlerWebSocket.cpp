/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>

#include "CNetHandlerWebSocket.hpp"

Net::CNetHandlerWebSocket::CNetHandlerWebSocket() {
    std::cout<<"Web socket handler is create\n";
}

std::string Net::CNetHandlerWebSocket::handle(const std::string &head, 
                                              bool &isSendAnswer) {
    if(head.substr(0,24) != "GET /setConnect HTTP/1.1") {
        return std::string();
    }
    std::string answerHead;
    answerHead += "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
    answerHead += "Upgrade: WebSocket\r\n";
    answerHead += "Connection: Upgrade\r\n";
    answerHead += "WebSocket-Origin: http://localhost:6000\r\n";
    answerHead += "WebSocket-Location: ws://localhost:6000\r\n\r\n";
    isSendAnswer = true;
    return answerHead;
}

unsigned int Net::CNetHandlerWebSocket::getSize (const std::string &dataFragment) {
    if(dataFragment.size() == 0) {
        return 0;
    }
    auto iter = dataFragment.begin();
    uint size = 0;
    while(((*iter) & '\x80') == '\x80') {
        ++iter;

        if (iter == dataFragment.end()) {
            return 0;
        }

        size <<= 7;
        size |= uint((*iter) & 0x7F);
    };
    return size;
}

std::string Net::CNetHandlerWebSocket::packData(const std::string &data) {
    std::string result("\x80");
    uint size = data.size();
    for(uint i = 4; i>0; i--) {
        char tmp = (size >> 7*i) & 0x7F;
        if (tmp != 0)
            result += (tmp | char(0x80));
    }
    result += char(size & 0x7F);
    result += data;
    return result;
}

std::string Net::CNetHandlerWebSocket::unpackData(const std::string &data) {
    if(data.size() <= 1) {
        return std::string();
    }

    auto iter = data.begin();
    uint size = 0;
    while(((*iter) & '\x80') == '\x80') {
        ++iter;

        if (iter == data.end()) {
            throw HandlerException::unexpectedEnding;
        }

        size <<= 7;
        size |= uint((*iter) & 0x7F);
    };

    if (size == 0) {
        return std::string();
    }

    std::string result;
    result.insert(result.end(), ++iter, data.end());
    return result;
}
