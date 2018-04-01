/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include "CSettingsManager.hpp"

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

    // Load parametrs
    std::string ip;
    int maxCon, port;
    if(!CSettingsManager::instance().getParametr("server",
                                                 "ip",
                                                 ip)) {
        ip = "127.0.0.1";
        CSettingsManager::instance().setParametr("server",
                                                 "ip",
                                                 ip);
    }
    if(!CSettingsManager::instance().getParametr("server",
                                                 "port",
                                                 port)) {
        port = 3000;
        CSettingsManager::instance().setParametr("server",
                                                 "port",
                                                 "3000");
    }
    if(!CSettingsManager::instance().getParametr("server",
                                                 "max_conections",
                                                 maxCon)) {
        maxCon = 10;
        CSettingsManager::instance().setParametr("server",
                                                 "max_conections",
                                                 "10");
    }

	while(reception_ == nullptr) {
		try {
			reception_ = new CReception (maxCon,
                                         ip.c_str(),
                                         port);
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