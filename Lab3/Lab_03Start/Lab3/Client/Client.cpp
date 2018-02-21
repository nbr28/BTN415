#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <fstream>
using namespace std;

void main()
{
	std::ofstream ofs("Output.txt");
	//starts Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		ofs << "ERROR: Failed to start WSA" << std::endl;
		return;
	}

	//initializes socket. SOCK_STREAM: TCP
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		ofs << "ERROR:  Failed to create ClientSocket" << std::endl;
		return;
	}

	//Connect socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						//Address family type itnernet
	SvrAddr.sin_port = htons(27000);					//port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address
	if ((connect(ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		ofs << "ERROR:  Connection attempted failed" << std::endl;
		return;
	}


	//receives Rxbuffer

	while (1) {
		//sends Txbuffer		
		char TxBuffer[128] = {};
		cout<< "Enter a file to transmit" << std::endl;
		ofs << "Enter a file to transmit" << std::endl;
		cin >> TxBuffer;
		std::ifstream ifs;
		ifs.open(TxBuffer);
		if (ifs)
		{
			char transmit = '\0';
			while (!ifs.eof())
			{
				ifs.get(transmit);
				send(ClientSocket, &transmit, sizeof(transmit), 0);
			}
		}
		else
		{
			cout << "File could not be opened" << std::endl;
			ofs << "File could not be opened" << std::endl;
		}
		
	}

	//closes connection and socket
	closesocket(ClientSocket);
	//frees Winsock DLL resources
	WSACleanup();
}
