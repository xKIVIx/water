/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <vector>

#include "CSettingsManager.hpp"
#include "CWaterCompute.hpp"

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
			std::cout << ".";
			std::this_thread::sleep_for( std::chrono::seconds(5) );
		}
	}
	std::cout << "\nServer is create.\n";
    CWaterCompute waterCompute;
    while( !isEnd() ) {
        STask task = reception_->getTask();
        if((task.token_.socket_ == -1) || 
           (task.data_.empty())) {
            continue;
        }
        std::vector <float> vertex;
        uint sizeVertexBlock = *( (size_t *)(task.data_.c_str() + 1) );
        vertex.reserve(sizeVertexBlock/4);
        vertex.insert(vertex.end(),
                      (float *)&(task.data_.c_str()[9]),
                      (float *)&(task.data_.c_str()[9]) + sizeVertexBlock / 4);
        
        std::vector <uint> face,
                           result;
        uint sizeFaceBlock = *( (size_t *)(task.data_.c_str() + 5) );
        face.reserve(sizeVertexBlock/4);
        face.insert(face.end(),
                    (uint *)&(task.data_.c_str()[9 + sizeVertexBlock]),
                    (uint *)&(task.data_.c_str()[9 + sizeVertexBlock]) + sizeFaceBlock / 4);
        
        waterCompute.loadData(vertex, face);
        waterCompute.computeWaterLvl(vertex, result);
        task.data_.clear();
        task.data_.reserve(result.size() * 3 * sizeof(uint32_t));
        for(auto iter = result.begin(); iter != result.end(); ++iter) {
            task.data_.insert(task.data_.end(),
                              (char *)&face.data()[*iter * 3],
                              (char *)&face.data()[*iter * 3 + 3]);
        }
	    reception_->compliteTask(task);
        waterCompute.clear();
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