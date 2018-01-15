#include "oop_udp_winsock.h"

//starts all necessary Windwos dlls
void udp_winsock::start_DLLS() {
	if ((WSAStartup(MAKEWORD(this->version_num1, this->version_num2), &this->wsa_data)) != 0) {
		Print("Could not start DLLs");
		std::cin.get();
		exit(0);
	}
}

//initializes a socket and returns it
SOCKET udp_winsock::initialize_tcp_socket() {
	SOCKET LocalSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (LocalSocket == INVALID_SOCKET) {
		WSACleanup();
		Print("Could not initialize socket");
		std::cin.get();
		exit(0);
	}
	return LocalSocket;
}

//base constructor (called for all objects)
//note that in case multiple socket objects are created, start_DLLS()
//is called multiple times. This, however, does not result in errors
//just a slight redundancy
udp_winsock::udp_winsock() {
	this->version_num1 = 2;
	this->version_num2 = 2;
	this->start_DLLS();
}