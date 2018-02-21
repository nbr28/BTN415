#include <windows.networking.sockets.h>
#include <iostream>
#include <fstream>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void threadLogic(SOCKET ConnectionSocket,ofstream& ofs)
{
	cout << "thread started" << endl;
	send(ConnectionSocket, "Ready to Receive", sizeof("Ready to Receive"), 0);
	while (1) {
		//receives RxBuffer
		char RxBuffer[128] = { '\0' };
		char TxBuffer[128] = "Ok";
		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
		cout << RxBuffer << endl;

		if (string(RxBuffer) == "done")
			break;

		send(ConnectionSocket, TxBuffer, sizeof(TxBuffer), 0);
	}

	cout << "closing connection" << endl;
	closesocket(ConnectionSocket);	//closes incoming socket
	cout << "Thread closed" << endl;
}

void main()
{
	std::ofstream ofs("Server_Output.txt");
	std::ofstream ofsFile = ofstream("temp.txt");
	if (!ofs)
		std::cout << "ERROR: Failed to open output file" << std::endl;
	if (!ofsFile)
		std::cout << "ERROR: Failed to open output temp file" << std::endl;

	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ofs << "ERROR:  Failed to start WSA" << std::endl;
		return;
	}

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		ofs << "ERROR:  Failed to create ServerSocket" << std::endl;
		return;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (::bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		ofs << "ERROR:  Failed to bind ServerSocket" << std::endl;
		return;
	}


	//listen on a socket
	if (listen(ServerSocket, 2) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		ofs << "ERROR:  listen failed to configure ServerSocket" << std::endl;
		return;
	}

	while (1)
	{




		cout << "Waiting for client connection\n" << endl;

		//accepts a connection from a client
		SOCKET ConnectionSocket;
		ConnectionSocket = SOCKET_ERROR;
		if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
			closesocket(ServerSocket);
			WSACleanup();
			return;
		}

		ofs << "Connection Established" << std::endl;

		bool correctUser = false;


		for (int i = 0; i < 3 && correctUser == false; i++)
		{
			char TempRxBuffer[128] = {};
			send(ConnectionSocket, "Login Name", sizeof("Login Name"), 0);
			recv(ConnectionSocket, TempRxBuffer, sizeof(TempRxBuffer), 0);

			if (strcmp(TempRxBuffer, "sam") == 0)
			{
				correctUser = true;
			}
		}


		if(correctUser)
		{
			//spawn new threads here
			std::thread(threadLogic,move(ConnectionSocket), ref(ofs)).detach();
		}
		else
		{
			send(ConnectionSocket, "failed", sizeof("failed"), 0);
			cout << "failed Connection" << endl;
			cout << "closing connection" << endl;
			closesocket(ConnectionSocket);	//closes incoming socket
		}



		
	}

	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources
}



