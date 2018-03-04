/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CNET
#define CNET

#include <string>
#include <vector>

#include "Handler/CNetHandler.hpp"

typedef unsigned int uint;

namespace Net {

    /**
     * @brief Generated exceptions.
     * 
     */
    enum Exception {
        listenSocetInitError,
        listenSocetError
    };

    /**
     * @brief Structure for storing connection data.
     * 
     */
    struct SToken {
        int socket_;
        uint connectionType_;
    };

    /**
     * @brief Server initialization class.
     * 
     */
    class CNet {
    public:
        /**
         * @brief Constructor. In case of errors, he throws exceptions.
         * @see Exception
         * 
         */
        CNet();
         
        /**
         * @brief Destructor.
         * 
         */
        ~CNet();

        /**
         * @brief The function of connection waiting.
         * 
         * @return SToken Data about the client. 
         *                If socket_ = -1, then a client 
         *                receive error occurred.
         */
        SToken acceptClient();

        /**
         * @brief Receiving data from the client.
         * 
         * @param clientToken Information about the client.
         * @param data Data.
         * @param dataSize Size of data.
         */
        void getData(const SToken &clientToken,
                     char **data,
                     uint &dataSize);

        /**
         * @brief Sending data to the client.
         * 
         * @param clientToken Information about the client.
         * @param data Data.
         * @param dataSize Size of data.
         */
        void sendData(const SToken &clientToken,
                      const char *data,
                      const uint dataSize);
    private:
        std::vector <CNetHandler *> handlers_;
        int listenSocket_ = -1;
    };
}


#endif // CNET