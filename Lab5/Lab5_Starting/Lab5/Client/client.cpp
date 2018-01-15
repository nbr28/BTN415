#include "oop_udp_winsock.h"
#include <iostream>
#include <fstream>

void Print(std::string msg, std::ofstream *ofs)
{
	std::cout << msg << std::endl;
	*ofs << msg << std::endl;
}

int main() {
	std::ofstream ofs("Client_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screenshots requires" << std::endl;
	
	Print("Creating Client Socket", &ofs);
	udp_winsock_client client(27000, "127.0.0.1", &ofs);

	char tx_message[128] = "Hello World";
	client.send_message_to(tx_message,27000, "127.0.0.1");

	WSACleanup();
	exit(0);
}
