#ifndef UDP_WINSOCK_H
#define UDP_WINSOCK_H

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>

class udp_winsock {
protected:
	std::ofstream *ofs;
	int version_num1, version_num2;
	int port;
	std::string ip;
	char rx_buffer[128];
	WSADATA wsa_data;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}

public:
	void start_DLLS();
	SOCKET initialize_tcp_socket();
	udp_winsock();
};

class udp_winsock_server : public udp_winsock {
protected:
	SOCKET server_socket;
	std::ofstream *ofs;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}

public:
	void bind_socket();
	char * receive_message_from(int, std::string);
	void send_message_to(char *, int, std::string);
	udp_winsock_server(int, std::string, std::ofstream *);
	~udp_winsock_server();
};

class udp_winsock_client : public udp_winsock {
protected:
	SOCKET client_socket;
	std::ofstream *ofs;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}
public:
	char * receive_message_from(int, std::string);
	void send_message_to(char *, int, std::string);
	udp_winsock_client(int, std::string, std::ofstream *);
	~udp_winsock_client();
};

#endif UDP_WINSOCK_H
