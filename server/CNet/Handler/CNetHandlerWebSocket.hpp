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
         * @see Net::CNetHandler::handle 
         */
        virtual std::string handle(const std::string &head,
                                   bool &isSendAnswer);

        /**
         * @see Net::CNetHandler::getSize 
         */
        virtual unsigned int getSize(const std::string &dataFragment);
        
        /**
         * @see Net::CNetHandler::packData 
         */
        virtual std::string packData(const std::string &data);

        /**
         * @see Net::CNetHandler::unpackData 
         */
        virtual std::string unpackData(const std::string &dataPack);
    };
};

#endif // CNET_HANDLER_WEB_SOCKET