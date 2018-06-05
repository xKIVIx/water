/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CSERVER
#define CSERVER

#include "CReception.hpp"
#include "CWaterCompute.hpp"
/**
 * @brief The class is a stream. which
 *        performs the work of the server.
 * 
 */
class CServer {
    public:
    
    CServer();

    ~CServer();

    private:
    CWaterCompute waterCompute;
    std::thread *workThread_ = nullptr;
    std::mutex mutex_;
    bool endState_ = false;
    CReception *reception_ = nullptr;
    
    /**
     * @brief Thread-safe end of work test.
     * 
     * @return true Break work.
     * @return false Continue work.
     */
    bool isEnd();

    /**
     * @brief Thread-safe job verification.
     * 
     * @return true Have job.
     * @return false Haven`t job.
     */
    bool isEmptyQueue();

    /**
     * @brief Flow function.
     * 
     */
    void work();

    /**
     * @brief Stop processing.
     * 
     */
    void stop();

    int computeWaterLvl(const std::string &data,
                              uint32_t    readPos,
                              std::string &result);
};

#endif // CSERVER