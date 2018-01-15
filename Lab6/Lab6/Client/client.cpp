#include "oop_winsock.h"
#include <fstream>
#include <string>

int main() {
	std::ofstream ofs("Server_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screen shots required" << std::endl;

	winsock_client client(27000, "127.0.0.1", &ofs);

	client.connect_to_tcp_server_loop();

	/*
    float my_array[4] = {1.25, 2.45, 1.87, 2.2};
    client.send_float_array(my_array);
	
    packet my_packet;
    my_packet.student_number = 640;
    strcpy_s(my_packet.student_name,"john smith");
    my_packet.student_gpa = 2.9;
    client.send_packet(my_packet);
	
	
    bitpacket my_bitpacket;
    my_bitpacket.student_number = 640;
    strcpy_s(my_bitpacket.student_name,"john smith");
    my_bitpacket.current = 1;
    my_bitpacket.doing_coop = 0;
    my_bitpacket.academic_violations = 0;
    my_bitpacket.student_gpa = 2.9;
    client.send_bitpacket(my_bitpacket);
	*/
	
	WSACleanup();
	exit(0);
}
