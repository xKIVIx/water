/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <ctime>
#include <regex>
#include <algorithm>

#include "libwshandshake.hpp"
#include "CNetHandlerWebSocket.hpp"

// head flags
#define FIN_FLAG 0x80
#define OPCOD_FLAG 0xf
#define MASK_FLAG 0x80
#define DATA_SIZE_FLAG 0x7f

/**
 * @brief Header options.
 * 
 */
enum HeadParametr {
    HAVE_MASK = 0x1
};

/**
 * @brief Operation codes.
 * 
 */
enum OpCode {
    EMPTY = 0,
    TEXT = 0x1,
    BINARY = 0x2,
    DISCONNECT = 0x8,
    PING = 0x9,
    PONG = 0xA
};

/**
 * @brief Regular expressions for finding the
 *        number to generate a confirmation code.
 * 
 */
const std::regex regexFindKey("[^ ]+==");

// heads sizes
const uint headSizeVBD = 10;
const uint headSizeBD = 4;
const uint headSizeD = 2;
const uint maskSize = 4;

void useMask(std::string &data, const std::string &mask) {
    uint i = 0;
    for(auto iter = data.begin(); iter != data.end(); ++iter) {
        *iter ^= mask[i];
        i = (i + 1)%mask.size();
    }
}

using namespace Net;

CNetHandlerWebSocket::CNetHandlerWebSocket() {
    std::cout << "Web socket handler is create\n";
}

std::string CNetHandlerWebSocket::handle(const std::string &head,
                                         bool &isSendAnswer) {
    if (head.substr(0, 24) != "GET /setConnect HTTP/1.1") {
        return std::string();
    }

    std::smatch sreachMach;
    std::regex_search(head, sreachMach, regexFindKey);
    std::string key = sreachMach.str();
    char answerHash[28];
    WebSocketHandshake::generate(key.c_str(), answerHash);

    std::string answerHead;
    answerHead += "HTTP/1.1 101 Switching Protocols\r\n";
    answerHead += "Upgrade: websocket\r\n";
    answerHead += "Connection: Upgrade\r\n";
    answerHead += "Sec-WebSocket-Accept: ";
    answerHead.insert(answerHead.end(),
                      answerHash,
                      answerHash + 28);
    answerHead += "\r\n\r\n\0";
    
    isSendAnswer = true;
    return answerHead;
}

SMessageHead CNetHandlerWebSocket::decodMessageHead(const std::string &messageHead) {
    std::cout << "Decoding message head.\n";
    SMessageHead result;

    // decode message type
    char opCode = messageHead[0] & OPCOD_FLAG;
    if(opCode == OpCode::PING) {
        result.type_ = MessageType::PING;
        result.headSize_ = headSizeD;
        return result;
    }
    if(opCode == OpCode::PONG) {
        result.type_ = MessageType::PONG;
        result.headSize_ = headSizeD;
        return result;
    }
    if(opCode == OpCode::DISCONNECT) {
        result.type_ = MessageType::DISCONNECT;
        result.headSize_ = headSizeD;
        return result;
    }

    if((messageHead[0] & FIN_FLAG) == 0) {
        result.type_ = MessageType::DATA_FRAGMENT;
    } else {
        result.type_ = MessageType::DATA;
    }

    if((messageHead[1] & MASK_FLAG) != 0) {
        result.parametrs_ |= HeadParametr::HAVE_MASK;
        result.headSize_ += maskSize;
    }

    // decode message size
    char sizeFlag = messageHead[1] & DATA_SIZE_FLAG;
    if(sizeFlag == DATA_SIZE_FLAG) {
        if( *(uint *)&messageHead.c_str()[2] != 0) {
            result.type_ = MessageType::ERROR_SIZE;
            std::cout << "Not support 64bit size.\n";
            return result;
        }
        *((char *)&result.dataSize_) = messageHead[6];
        *((char *)&result.dataSize_ + 1) = messageHead[7];
        *((char *)&result.dataSize_ + 2) = messageHead[8];
        *((char *)&result.dataSize_ + 3) = messageHead[9];
        result.headSize_ += headSizeVBD;
        return result;
    }
    if(sizeFlag == 0x7e) {
        *(((char *)&result.dataSize_) + 1) = messageHead[2];
        *((char *)&result.dataSize_) = messageHead[3];
        result.headSize_ += headSizeBD;
        return result;
    }
    result.dataSize_ += uint(sizeFlag);
    result.headSize_ += headSizeD;
    return result;
}

std::string CNetHandlerWebSocket::disconect(DisconnectReason reason) {
    std::string result;
    result += char(0x88);
    result += char(0x4);
    if(reason == DisconnectReason::DATA_ERROR) {
        result += "1003";
    } else if(reason == DisconnectReason::SERVER_ERROR) {
        result += "1001";
    } else {
        result += "1000";
    }
    return result;
}

uint CNetHandlerWebSocket::getMessageHeadMaxSize() {
    return headSizeVBD;
}

std::string CNetHandlerWebSocket::packData(const std::string &data,
                                           const bool isUseMask) {
    std::string result;
    result += char(FIN_FLAG | OpCode::BINARY);
    uint dataSize = data.size();
    if(dataSize > 65536) {
        result += char(DATA_SIZE_FLAG);
        char empty [] = {0,0,0,0};
        result.insert(result.end(), 
                      empty, 
                      empty + 4);
        result += *(((char *)&dataSize)+3);
        result += *(((char *)&dataSize)+2);
        result += *(((char *)&dataSize)+1);
        result += *((char *)&dataSize);

    } else if(dataSize > 125) {
        result += char(0x7e);
        result += *(((char *)&dataSize)+1);
        result += *((char *)&dataSize); 
    } else {
        result += char(dataSize);
    }

    if(isUseMask == true) {
        srand( time(0) );
        std::string mask;

        mask += rand();
        mask += rand();
        mask += rand();
        mask += rand();

        std::string tmp = data;
        useMask(tmp, mask);
        result += mask;
        result += tmp;
        result[1] |= MASK_FLAG;
    } else {
        result += data;
    }

    return result;
}

std::string CNetHandlerWebSocket::ping() {
    std::string result;
    result += char(FIN_FLAG | OpCode::PING);
    return result;
}

std::string CNetHandlerWebSocket::pong() {
    std::string result;
    result += char(FIN_FLAG | OpCode::PONG);
    return result;
}

std::string CNetHandlerWebSocket::unpackData(const SMessageHead &head,
                                             const std::string &dataPack) {
    if((head.parametrs_ & HeadParametr::HAVE_MASK) == 0) {
        return dataPack.substr(head.headSize_, 
                            head.dataSize_);
    } else {
        std::string mask = dataPack.substr(head.headSize_ - maskSize, 
                                           maskSize);
        std::string result = dataPack.substr(head.headSize_, 
                                             head.dataSize_);
        useMask(result, mask);
        return result;
    }
}