/* Copyright (c) 2018, Aleksandrov Maksim */

#include <fstream>
#include <regex>

#include "CSettingsManager.hpp"

const char regFindNameParametr[] = "[\\w]+(?=[ ]*=)";
const char regFindDataParametr[] = "[\\w.]+";

using namespace std;

CSettingsManager *CSettingsManager::manegerPtr_ = nullptr;

void parseFile(ifstream *f,
               std::map<std::string, std::string> &r) {
    regex regExName(regFindNameParametr),
        regExData(regFindDataParametr);
    smatch match;
    string sbuffer;
    sbuffer.reserve(256);

    while( !f->eof() ) {
        char buffer[256];
        f->getline(buffer, 256);
        sbuffer = buffer;
        if( !regex_search(sbuffer, 
                          match, 
                          regExName) ) {
            continue;
        }
        string nameP = match.str();
        sbuffer = match.suffix();
        if( !regex_search(sbuffer, 
                          match, 
                          regExData) ) {
            continue;
        }
        string dataP = match.str();
        r.insert( pair<string, string> (nameP, dataP) );
    }

}

bool CSettingsManager::getParametr(const std::string &category,
                                   const std::string &dataName,
                                   std::string &parametr) {
    lock_guard<mutex> lock(manegerMutex_);
    auto iter = data_.find(category);
    if(iter == data_.end()) {
        ifstream file(path_ + category + ".conf");
        if( file.is_open() ) {

            iter = data_.insert(pair<string,map<string,string>>(category,
                                                                map<string,string>()
                                                                )).first;
            parseFile(&file, 
                      iter->second);
        } else {
            return false;
        }
    }

    auto dataIter = iter->second.find(dataName);
    if(dataIter == iter->second.end()) {
        return false;
    }
    parametr = dataIter->second;
    return true;
}

bool CSettingsManager::getParametr(const std::string &category,
                                   const std::string &dataName,
                                   int &parametr) {
    std::string data;
    if(!getParametr(category,
                    dataName,
                    data)) {
        return false;
    }
    try {
        parametr = stoi(data.c_str());
    } catch(exception e) {
        return false;
    }
    return true;
}


bool CSettingsManager::saveParametrs() {
    lock_guard<mutex> lock(manegerMutex_);
    for(auto categoryIter = data_.begin();
        categoryIter != data_.end();
        ++categoryIter) {

        ofstream file(path_ + categoryIter->first + ".conf");
        if( !file.is_open() ) {
            return false;
        }
        for(auto parametrIter = categoryIter->second.begin();
            parametrIter != categoryIter->second.end();
            ++parametrIter) {
            string writeData = parametrIter->first +
                               " = " +
                               parametrIter->second +
                               "\r\n";
            file.write(writeData.c_str(),
                       writeData.size());
        }
        file.close();
    }
    return true;
}

void CSettingsManager::setPath(const std::string &path) {
    lock_guard<mutex> lock(manegerMutex_);
    path_ = path;
}

bool CSettingsManager::setParametr(const std::string &category,
                                   const std::string &dataName,
                                   const std::string &parametr) {
    lock_guard<mutex> lock(manegerMutex_);
    auto iter = data_.find(category);
    if(iter == data_.end()) {
        ifstream file(path_ + category + ".conf");
        if( file.is_open() ) {

            iter = data_.insert(pair<string,map<string,string>>(category,
                                                                map<string,string>()
                                                                )).first;
            parseFile(&file, 
                      iter->second);
        } else {
            iter = data_.insert(pair<string, map<string, string>>(category,  
                                                                  map<string, string>())).first;
        }
    }

    iter->second.insert_or_assign( dataName, parametr );
    return true;

}
CSettingsManager &CSettingsManager::instance() {
    if(manegerPtr_ == nullptr) {
        manegerPtr_ = new CSettingsManager();
    }
    return *manegerPtr_;
}
void CSettingsManager::detach() {
    if(manegerPtr_ != nullptr) {
        delete manegerPtr_;
        manegerPtr_ = nullptr;
    }
}
CSettingsManager::CSettingsManager() {

};
CSettingsManager::~CSettingsManager() {

};