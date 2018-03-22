#include "../Client/oop_winsock.h"
#include <fstream>
#include <string>

void test_float(float * my_input) {
	float test[4] = { 1.3, 1.2, 1.1, 3.6 };
	memcpy(my_input, test, sizeof(int) * 4);
}

int main() {

	float my_array[4];
	bitPacket p;
	test_float(my_array);

	std::ofstream ofs("Server_Output.txt");
	if(!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screen shots required" << std::endl;

	winsock_server server(27000,"127.0.0.1", &ofs);

	server.accept_connection();

	
	float rec_array[4] = {0};
	server.receive_float_array(rec_array);
	
	packet rec_packet;
	server.receive_packet(rec_packet);


	bitPacket rec_bitpacket;
	server.receive_bitPacket(rec_bitpacket);

	
	WSACleanup();
	exit(0);
}
