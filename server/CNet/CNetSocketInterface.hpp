/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CNET_SOCKET_INTEFACE
#define CNET_SOCKET_INTEFACE

#define CNET_DOMAIN AF_INET

typedef unsigned int uint;

namespace Net {
    /**
     * @brief The interface for accessing sockets.
     *        At the beginning of using the functions, 
     *        you need to initialize. @see CNetSocketInterface::initInterface
     *        At the end of work, call closeInterface.
     * 
     */
    class CNetSocketInterface {
    public:
        
        /**
         * @brief Create a server socket.
         * 
         * @param maxCon The maximum number of connections.
         * @param port 
         * @param ipAdress IPV4
         * @return int Listen socket.
         */
        static int createServerSocket(const uint maxCon,
                                      const int port,
                                      const char *ipAdress);

        /**
         * @brief Create a client socket.
         * 
         * @param port 
         * @param ipAdress IPV4
         * @return int Socket.
         */                              
        static int createClientSocket(const int port,
                                      const char *ipAdress);

        /**
         * @brief Waiting for the client.
         * 
         * @param socket Listen socket.
         * @return int Client socket.
         */
        static int accept(const int socket);

        /**
         * @brief Close socket.
         * 
         * @param socket Socket.
         */
        static void closeSocket(const int socket);

        /**
         * @brief Interface initialization function.
         * 
         */
        static void initInterface();

        /**
         * @brief Function of disabling the initialized interface.
         * 
         */
        static void closeInterface();

        /**
         * @brief Sending data.
         * 
         * @param socket 
         * @param data 
         * @param dataSize 
         * @return int The size of the sent data. 
         *             -1 if an error occurred.
         */
        static int send (const int socket,
                         const char *data,
                         const int dataSize);

        /**
         * @brief Receiving data.
         * 
         * @param socket 
         * @param buffer 
         * @param bufferSize 
         * @return int The size of the data received.
         *             -1 if an error occurred.
         */
        static int recv (const int socket,
                         char *buffer,
                         const int bufferSize);
    };
};

#endif // CNET_SOCKET_INTEFACE