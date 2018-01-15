#include <iostream>
#include <fstream>
#include "transport.h"

using namespace std;

int main() {
	ofstream ofs("Output.txt");
	if (ofs.fail())
		std::cout << "Failed to create output file - Screenshots will be required" << std::endl;

	string message = "A cool message to be transmitted.";
	channel wireless(20, 20);

	udp_client client1(80);
	udp_server server1(80, message);

	client1.request_transmission(&server1, &wireless);
	client1.print_data(ofs);
	cout << endl;

	// tcp_client client2(80);
	// tcp_server server2(80,message);

	// if (client2.three_way_handshake(&server2,&wireless)){
	//     ofs << "Connection Established." << endl;
	//     client2.request_transmission(&server2,&wireless);
	//     if (client2.four_way_handshake(&server2,&wireless)){
	//         ofs << "Connection Closed." << endl;
	//         client2.print_data(ofs);
	//     }
	// }

	ofs.close();
	return 0;
}