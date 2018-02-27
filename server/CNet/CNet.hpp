/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CNET
#define CNET

namespace Net {
    #define CNET_DOMAIN AF_INET
    #define CNET_PORT 6000
    /**
     * @brief Generated exceptions.
     * 
     */
    enum Exception {
        listenSocetInitError,
        listenSocetBindError
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
         * @brief The connection waiting function.
         * 
         * @return int client socket.
         */
        int listen();
    private:
        int listenSocket_ = -1;
        void * adrr_ = nullptr;
    };
}


#endif