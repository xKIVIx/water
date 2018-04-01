/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <string>

#include "CServer.hpp"
#include "CNet/CNetSocketInterface.hpp"
<<<<<<< HEAD
#include "CSettingsManager.hpp"
=======
>>>>>>> 5a961d3f4e90161e4a1a782ca65c44780368dfae

int main(int argc, char *argv[]) {
    Net::CNetSocketInterface::initInterface();
	CServer server;
	char command [256];
	while(true) {
		std::cin >> command;
		if(std::string(command) == std::string("stop")) {
			std::cout << "Server stop\n";
            break;
		}
	}
    Net::CNetSocketInterface::closeInterface();
    CSettingsManager::instance().saveParametrs();
    CSettingsManager::instance().detach();
	return 0;
}