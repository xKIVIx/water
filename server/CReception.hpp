/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CRECEPTION
#define CRECEPTION

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "CNet/CNet.hpp"

/**
 * @brief Structure for storing task data.
 * 
 */
struct STask {
    Net::SToken token_;
    uint dataSize_ = 0;
    char * data_ = nullptr;
};

/**
 * @brief A class for asynchronous 
 *        task acceptance from clients.
 * 
 */
class CReception {
public:
    /**
     * @brief Constructor.
     * 
     * @param maxCon Maximum number of connections.
     * @param ip IPv4.
     * @param port Port number. 
     */
    CReception(const int maxCon,
               const char *ip,
               const int port);
    
    /**
     * @brief Destructor.
     * 
     */
    ~CReception();
    
    /**
     * @brief Getting the task.
     * 
     * @return STask
     */
    STask getTask();

    /**
     * @brief Sending the completed task.
     * 
     * @param task Сompleted task.
     */
    void compliteTask(STask &task);

private:
    std::queue <STask> taskQueue;

    std::mutex mutexTaskQueue_,
               mutexEndState_;

    std::condition_variable comeQuestEvent_;

    std::thread *workThread_ = nullptr;

    Net::CNet *connection_ = nullptr;

    bool endState_ = false;

    bool isEnd();

    bool isEmptyQueue();

    void work();
};

#endif // CRECEPTION