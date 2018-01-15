#include <iostream>
#include <fstream>
#include "../Client/oop_udp_winsock.h"

void Print(std::string msg, std::ofstream *ofs)
{
	std::cout << msg << std::endl;
	*ofs << msg << std::endl;
}

int main() {
	std::ofstream ofs("Server_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screenshots required" << std::endl;

	Print("Creating Server Socket", &ofs);
	udp_winsock_server server(27000, "127.0.0.1", &ofs);
	Print("Waiting for Message", &ofs);

	char *ptrResult = nullptr;
	ptrResult = server.receive_message_from(27000, "127.0.0.1");

	Print((std::string)ptrResult, &ofs);

	WSACleanup();
	exit(0);
}
