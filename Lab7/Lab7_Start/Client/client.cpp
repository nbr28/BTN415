#include "oop_winsock.h"
#include <iostream>
#include <fstream>

int main() {

	std::ofstream ofs("Client_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file" << std::endl;

	winsock_client client(27000, "127.0.0.1", &ofs);

	client.connect_to_tcp_server_loop();

	
    frame tx_frame;
    tx_frame.length = 3;
	tx_frame.body = new int[tx_frame.length];
	tx_frame.body[0] = 3;
	tx_frame.body[1] = 650;
	tx_frame.body[2] = 2;
	tx_frame.tail = 5;
    client.send_frame(tx_frame);


	
	
	WSACleanup();
	exit(0);
}
