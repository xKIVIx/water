/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CNET_HANDLER
#define CNET_HANDLER

#include <string>


namespace Net {

    enum HandlerException {
        unexpectedEnding
    };

    /**
     * @brief Virtual class for creating a connection protocol handler
     * 
     */
    class CNetHandler {
    public:
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
         * @brief Determines the size of the data for the initial fragment.
         * 
         * @param dataFragment Determines the size of the incoming data 
         *                     from the initial fragment.
         * @return unsigned int 
         */
        virtual unsigned int getSize(const std::string &dataFragment) = 0;

        /**
         * @brief Packs the data in accordance with the transmission protocol.
         * 
         * @param data 
         * @return std::string Result.
         */
        virtual std::string packData(const std::string &data) = 0;

        /**
         * @brief Unpacks the data according to the transmission protocol.
         * 
         * @param dataPack 
         * @return std::string result
         */
        virtual std::string unpackData(const std::string &dataPack) = 0;
    };
};


#endif // CNET_HANDLER