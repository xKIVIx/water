/* Copyright (c) 2018, Aleksandrov Maksim */

#include "CNetHandler.hpp"

#ifndef CNET_HANDLER_WEB_SOCKET
#define CNET_HANDLER_WEB_SOCKET

namespace Net {
    /**
     * @brief The protocol handler of the WebSocket
     * 
     */
    class CNetHandlerWebSocket:CNetHandler {
    public:
        CNetHandlerWebSocket();

        /**
         * @see Net::CNetHandler::decodMessageHead 
         */
        virtual SMessageHead decodMessageHead(const std::string &messageHead);

        /**
         * @see Net::CNetHandler::disconect 
         */
        virtual std::string disconect(DisconnectReason reason);

        /**
         * @see Net::CNetHandler::handle 
         */
        virtual std::string handle(const std::string &head,
                                   bool &isSendAnswer);

        /**
         * @see Net::CNetHandler::getMessageHeadMaxSize 
         */
        virtual uint getMessageHeadMaxSize();
        
        /**
         * @see Net::CNetHandler::packData 
         */
        virtual std::string packData(const std::string &data,
                                     const bool isUseMask);

        /**
         * @see Net::CNetHandler::ping 
         */
        virtual std::string ping();
        
        /**
         * @see Net::CNetHandler::pong 
         */
        virtual std::string pong();        

        /**
         * @see Net::CNetHandler::unpackData 
         */
        virtual std::string unpackData(const SMessageHead &head,
                                       const std::string &dataPack);
    };
};

#endif // CNET_HANDLER_WEB_SOCKET