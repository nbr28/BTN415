#include "../Client/oop_winsock.h"
#include <iostream>
#include <fstream>

int main() {
	
	std::ofstream ofs("Server_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file" << std::endl;

	winsock_server server(27000,"127.0.0.1", &ofs);

	server.accept_connection();

	
	frame rec_frame;
	server.receive_frame(rec_frame);


	WSACleanup();
	exit(0);
}


