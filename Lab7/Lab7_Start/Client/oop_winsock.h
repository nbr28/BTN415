#ifndef WINSOCK_H
#define WINSOCK_H

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

struct frame {
	int length; //represents the number of elements in the body
	int *body;
	int tail;
};

const int emptyFrame = 8;

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
	void receive_frame(frame & refFrame);
	void send_frame(char *);  
	void send_message(frame& sendFrame);
	winsock_server(int, std::string, std::ofstream*);
	~winsock_server();
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
	void send_message(char*);
	void send_frame(frame& sendFrame);   //sends message to the client_socket
	void connect_to_tcp_server(); //tries to connect, exits if no server available
	void connect_to_tcp_server_loop(); //keeps trying to connect until successful
	winsock_client(int, std::string, std::ofstream*);
	~winsock_client();
};

std::string FrameString(frame& f);

#endif WINSOCK_H
