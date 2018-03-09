/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <string>

#include "CReception.hpp"

int main(int argc, char *argv[]) {
	CReception * reception;
	try {
		reception = new CReception (10, "127.0.0.1", 3000);
	} catch (Net::Exception ex) {
		std::cout<<ex<<std::endl;
		std::cin.get();;
		return ex;
	}
	STask quest = reception->getQuest();
	std::string dataCome;
	dataCome.reserve(quest.dataSize_);
	dataCome.insert(dataCome.end(), 
					quest.data_, 
					quest.data_+ quest.dataSize_);
	std::cout<<dataCome;
	reception->compliteQuest(quest);
	delete reception;
	std::cin.get();
	return 0;
}