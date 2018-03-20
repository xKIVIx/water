/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CNET_HANDLER
#define CNET_HANDLER

#include <string>


namespace Net {

    /**
     * @brief Types of messages received.
     * 
     */
    enum MessageType {
        PING,
        PONG,
        DATA,
        DISCONNECT,
        DATA_FRAGMENT,
        ERROR_SIZE, // error in the size of the 
                    // message when it is processed.
        ERROR_HEAD  // error message header when processing it.
    };

    /**
     * @brief Causes of bonding.
     * 
     */
    enum DisconnectReason {
        NORMAL,
        DATA_ERROR,
        SERVER_ERROR
    };

    /**
     * @brief Structure for storing the header of the decoded message.
     * 
     */
    struct SMessageHead {
        uint dataSize_ = 0,
             headSize_ = 0,
             parametrs_ = 0;
        MessageType type_ = ERROR_HEAD;
    };

    /**
     * @brief Virtual class for creating a connection protocol handler
     * 
     */
    class CNetHandler {
    public:
        /**
         * @brief Function for decoding the message header.
         * 
         * @param messageHead The part of the message or 
         *                    the entire message containing the title.
         * @return SMessageHead Decrypted message header.
         */
        virtual SMessageHead decodMessageHead(const std::string &messageHead) = 0;

        /**
         * @brief A function for correctly terminating a connection.
         * 
         * @param reason The cause of the connection failure.
         * @return std::string Package to send.
         */
        virtual std::string disconect(DisconnectReason reason) = 0;

        /**
         * @brief Puncturing header processing function. 
         *        Returns the response to the query. 
         *        If the protocol header does not apply to the current handler, 
         *        an empty response is returned.
         * 
         * @param head Protocol header for connection setup.
         * @param isSendAnswer Sets whether the response should be sent.
         * @return std::string Answer.
         */
        virtual std::string handle(const std::string &head,
                                   bool  &isSendAnswer) = 0;
        
        /**
         * @brief Get the maximum size of the message header.
         * 
         * @return uint Head size.
         */
        virtual uint getMessageHeadMaxSize() = 0;

        /**
         * @brief Packs the data in accordance with the transmission protocol.
         * 
         * @param data Data for packaging.
         * @param isUseMask Use the mask when packing.
         * @return std::string Package to send.
         */
        virtual std::string packData(const std::string &data,
                                     const bool isUseMask) = 0;

        /**
         * @brief Receive a package to send a ping request.
         * 
         * @return std::string Package to send.
         */
        virtual std::string ping() = 0;
        
        /**
         * @brief Receive a package to send a ping response.
         * 
         * @return std::string Package to send.
         */
        virtual std::string pong() = 0;

        /**
         * @brief Unpacks the data according to the transmission protocol.
         * 
         * @param head Decoded message header.
         * @param dataPack A message with the heading at the beginning.
         * @return std::string Unpacked data.
         */
        virtual std::string unpackData(const SMessageHead &head,
                                       const std::string &dataPack) = 0;
    };
};


#endif // CNET_HANDLER