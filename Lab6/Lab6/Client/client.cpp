#include "oop_winsock.h"
#include <fstream>
#include <string>

int main() {
	std::ofstream ofs("Server_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screen shots required" << std::endl;

	winsock_client client(27000, "127.0.0.1", &ofs);

	client.connect_to_tcp_server_loop();

	
    float my_array[4] = {1.25, 2.45, 1.87, 2.2};
    client.send_float_array(my_array,4);
	
    packet my_packet;
    my_packet.student_number = 640;
    strcpy_s(my_packet.student_name,"john smith");
    my_packet.student_gpa = 2.9;
    client.send_packet(my_packet);
	
	
    bitPacket my_bitPacket;
    my_bitPacket.student_num = 640;
    strcpy_s(my_bitPacket.student_name,"john smith");
    my_bitPacket.current = 1;
    my_bitPacket.doing_coop = 0;
    my_bitPacket.academic_violations = 0;
    my_bitPacket.student_gpa = 2.9;
    client.send_bitPacket(my_bitPacket);

	
	WSACleanup();
	exit(0);
}
