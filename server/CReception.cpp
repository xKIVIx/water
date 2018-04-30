/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>

#include "CReception.hpp"

CReception::CReception(const int maxCon,
                       const char *ip,
                       const int port) {
    connection_ = new Net::CNet(maxCon, ip, port);
    workThread_ = new std::thread(&CReception::work, this);   
}

void CReception::work() {
    Net::SToken clientToken;
    while( !isEnd() ) {
        std::cout << "Wait client\n";
        clientToken = connection_->acceptClient();
        if(clientToken.socket_ == -1) {
            continue;
        }
        STask task;
        task.token_ = clientToken;
        std::cout << "Wait task\n";
        if( !connection_->getData(clientToken, task.data_) ) {
            connection_->disconnect(clientToken,
                                    Net::DisconnectReason::NORMAL);
            std::cout << "Cann`t get data from message\n";
            continue;
        }
        if(task.data_.size() == 0) {
            connection_->disconnect(clientToken,
                                    Net::DisconnectReason::NORMAL);
            std::cout << "Empty data\n";
            continue;
        }
        std::cout << "Get task\n";
        mutexTaskQueue_.lock();
        taskQueue.push(task);
        mutexTaskQueue_.unlock();
        comeQuestEvent_.notify_one();
    }
}

bool CReception::isEnd() {
    std::lock_guard <std::mutex> lock (mutexEndState_);
    return endState_;
}

bool CReception::isEmptyQueue() {
    std::lock_guard <std::mutex> lock (mutexTaskQueue_);
    return taskQueue.empty();
}

CReception::~CReception() {
    stop();

    workThread_->join();
    delete workThread_;

    
    // clear task queue
    mutexTaskQueue_.lock();
    while( !taskQueue.empty() ) {
        connection_->disconnect(taskQueue.front().token_,
                                Net::DisconnectReason::SERVER_ERROR);
        taskQueue.pop();
    }
    mutexTaskQueue_.unlock();
    delete connection_;
}

STask CReception::getTask() {
    while( !isEnd() ) {
        if( !isEmptyQueue() ) {
            std::lock_guard <std::mutex> lock (mutexTaskQueue_);
            STask result = taskQueue.front();
            taskQueue.pop();
            return result;
        }
        std::mutex mutexEvent;
        std::unique_lock<std::mutex> locker(mutexEvent);
        comeQuestEvent_.wait( locker );
    }
    return STask();
}

void CReception::compliteTask(STask &task) {
    connection_->sendData(task.token_, 
                          task.data_);
    connection_->disconnect(task.token_,
                            Net::DisconnectReason::NORMAL);
    std::cout << "Task complite\n";
}

void CReception::stop() {
    mutexEndState_.lock();
    endState_ = true;
    mutexEndState_.unlock();
    // close listen socket
    connection_->close();
    // free waiting threads
    comeQuestEvent_.notify_all();
}