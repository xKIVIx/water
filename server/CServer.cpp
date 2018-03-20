/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>

#include "CServer.hpp"

CServer::CServer() {
    workThread_ = new std::thread(&CServer::work, this);
}

CServer::~CServer() {
    stop();
    workThread_->join();
    delete reception_;
    delete workThread_;
}

void CServer::work() {
    std::cout << "Creating server\n";
	while(reception_ == nullptr) {
		try {
			reception_ = new CReception (10, "127.0.0.1", 3000);
		} catch (Net::Exception ex) {
			if( isEnd() ) {
				return;
			}
			std::cout << ".\n";
			std::this_thread::sleep_for( std::chrono::seconds(5) );
		}
	}
	std::cout << "Server is create.\n";
    while( !isEnd() ) {
        STask task = reception_->getTask();
        if(task.token_.socket_ == -1) {
            continue;
        }
        // TODO
	    reception_->compliteTask(task);
    }	
}

void CServer::stop() {
    mutex_.lock();
    endState_ = true;
    mutex_.unlock();
    reception_->stop();
}

bool CServer::isEnd() {
    std::lock_guard <std::mutex> lock (mutex_);
    return endState_;
}