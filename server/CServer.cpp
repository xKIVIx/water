/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <vector>

#include "CSettingsManager.hpp"

#include "CServer.hpp"

#define errorMessage(message, failCode) std::cout << message << ": " << failCode << std::endl

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
    while( !isEnd() ) {
        STask task = reception_->getTask();
        if((task.token_.socket_ == -1) || 
           (task.data_.empty())) {
            continue;
        }
        uint8_t opCode = *(uint8_t *)task.data_.data();
        int err;
        std::string result;
        switch(opCode) {
            case 0:
                err = computeWaterLvl(task.data_, 1, result);
                break;
            default:
                break;
        }
        task.data_.swap(result);
	    reception_->compliteTask(task);
        
    }	
}

int CServer::computeWaterLvl(const std::string &data,
                                   uint32_t    readPos,
                                   std::string &result) {

    float waterVal = *(float *)(data.data() + readPos);
    readPos += 4;

    uint sizeVertexBlock = *(uint *)(data.data() + readPos);
    readPos += 4;

    uint sizeFaceBlock = *(uint *)(data.data() + readPos);
    readPos += 4;

    std::vector <float> vertex;
    vertex.reserve(sizeVertexBlock/4);
    vertex.insert(vertex.end(),
                  (float *)(data.data() + readPos),
                  (float *)(data.data() + readPos + sizeVertexBlock));
    readPos += sizeVertexBlock;

    std::vector <uint> face;
    face.reserve(sizeVertexBlock/4);
    face.insert(face.end(),
                (uint *)(data.data() + readPos),
                (uint *)(data.data() + readPos + sizeFaceBlock));

    int err = waterCompute.loadData(vertex, face);
    if(err != 0) {
        errorMessage("Fail load data", err);
        waterCompute.clear();
        return 0;
    }

    err = waterCompute.prepareData();
    if(err != 0) {
        errorMessage("Fail prepare data", err);
        waterCompute.clear();
        return 0;
    }

    waterCompute.addWater(waterVal);

    std::list<std::vector<float>> resultVertex;
    std::list<std::vector<uint32_t>> resultFaces;
    waterCompute.getWaterLvls(resultVertex, resultFaces);

    auto iterVertex = resultVertex.begin();
    auto iterFaces = resultFaces.begin();
    for(; iterVertex != resultVertex.end();) {
        if(iterVertex->empty()) {
            iterFaces++;
            iterVertex++;
            continue;
        }
        uint32_t tmp = iterVertex->size();
        result.insert(result.end(),
                      (char *)&tmp,
                      (char *)&tmp + sizeof(uint32_t));
        result.insert(result.end(),
                      (char *)iterVertex->data(),
                      (char *)iterVertex->data() + tmp * sizeof(float));

        tmp = iterFaces->size();
        result.insert(result.end(),
                      (char *)&tmp,
                      (char *)&tmp + sizeof(uint32_t));
        result.insert(result.end(),
                      (char *)iterFaces->data(),
                      (char *)iterFaces->data() + tmp * sizeof(float));

        iterFaces++;
        iterVertex++;
    }
    waterCompute.clear();
    return 0;
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