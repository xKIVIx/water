/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <string>
#include <chrono>

#include "CServer.hpp"

int main(int argc, char *argv[]) {
	CServer server;
	char command [256];
	while(true) {
		std::cin >> command;
		if(std::string(command) == std::string("stop")) {
			std::cout << "Server stop\n";
			return 0;
		}
	}
	return 0;
}