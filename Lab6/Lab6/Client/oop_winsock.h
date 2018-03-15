#ifndef WINSOCK_H
#define WINSOCK_H

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

struct packet
{
	int student_number;
	char student_name[32];
	float student_gpa;
};
const int bitPacketSize = 41;
struct bitPacket
{
	int student_num;
	char student_name[32];
	unsigned char current : 1;
	unsigned char doing_coop : 1;
	unsigned char academic_violations : 1;
	unsigned char padding : 5;
	float student_gpa;
};


class winsock {
protected:
	std::ofstream *ofs;
	int version_num1, version_num2; // determines the WSADATA version numbers
	int port; // port number
	std::string ip; //ip string e.g. "127.0.0.1"
	char rx_buffer[128]; //note that the rx_buffer has only 128 bytes
	WSADATA wsa_data;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}
public:
	void start_DLLS();
	SOCKET initialize_tcp_socket();
	winsock();
};

class winsock_server : public winsock {
protected:
	SOCKET server_socket, connection_socket;
	std::ofstream *ofs;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}
public:
	void bind_socket();
	void listen_socket();
	void accept_connection();
	void close_connection();
	char * receive_message();  
	void send_message(char *);  
	winsock_server(int, std::string, std::ofstream*);
	~winsock_server();
	void receive_float_array(float arr[]);
	void receive_packet(packet& pack);
	void receive_bitPacket(bitPacket &pack);
};

class winsock_client : public winsock {
protected:
	SOCKET client_socket;
	std::ofstream *ofs;

	void Print(std::string msg)
	{
		std::cout << msg << std::endl;
		*ofs << msg << std::endl;
	}
public:
	char * receive_message(); //receives message from the client_socket
	void send_message(char *); //sends message to the client_socket
	void connect_to_tcp_server(); //tries to connect, exits if no server available
	void connect_to_tcp_server_loop(); //keeps trying to connect until successful
	winsock_client(int, std::string, std::ofstream*);
	~winsock_client();
	void send_float_array(float arr[], int size);
	void send_packet(packet& pack);
	void send_bitPacket(bitPacket &pack);
};

#endif WINSOCK_H
