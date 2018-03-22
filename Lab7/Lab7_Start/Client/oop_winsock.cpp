#include "oop_winsock.h"

//starts all necessary Windwos dlls
void winsock::start_DLLS() {
	if ((WSAStartup(MAKEWORD(this->version_num1, this->version_num2), &this->wsa_data)) != 0) {
		Print("Could not start DLLs");
		std::cin.get();
		exit(0);
	}
}

//initializes a socket and returns it
SOCKET winsock::initialize_tcp_socket() {
	SOCKET LocalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
winsock::winsock(){
	this->version_num1 = 2;
	this->version_num2 = 2;
	this->start_DLLS();
}

//binds the server socket
void winsock_server::bind_socket(){
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET; //Address family type internet
	SvrAddr.sin_port = htons(this->port); //port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str()); //IP address
	if ((bind(this->server_socket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(this->server_socket);
		WSACleanup();
		Print("Could not bind to the socket");
		std::cin.get();
		exit(0);
	}
}

//puts the server socket in listening mode
void winsock_server::listen_socket(){
	if (listen(this->server_socket, 1) == SOCKET_ERROR) {
		closesocket(this->server_socket);
		WSACleanup();
		Print("Could not listen to the provided socket.");
		std::cin.get();
		exit(0);
	}
	else {
		Print("Waiting for client connection");
	}
}

//accespts incoming connections
void winsock_server::accept_connection(){
	if ((this->connection_socket = accept(this->server_socket, NULL, NULL)) == SOCKET_ERROR) {
		closesocket(this->server_socket);
		WSACleanup();
		Print("Could not accept incoming connection.");
		std::cin.get();
		exit(0);
	}
	else {
		Print("Connection Accepted");
	}
}

//server destructor that closes all sockets opened by this object
void winsock_server::close_connection() {
	closesocket(this->connection_socket); //closes incoming socket
}

//receives messages from the connection_socket
char * winsock_server::receive_message() {
	memset(rx_buffer, 0, 128);
	recv(this->connection_socket, this->rx_buffer, sizeof(this->rx_buffer), 0);

	std::string msg = "Received: " + (std::string)rx_buffer;
	Print(msg);
	return this->rx_buffer;
}

void winsock_server::receive_frame(frame & refFrame)
{
	recv(this->connection_socket, this->rx_buffer, sizeof(this->rx_buffer), 0);
	
	memcpy(&refFrame.length, rx_buffer, sizeof(int));//copy over length of array
	refFrame.body = new int[refFrame.length];
	if (refFrame.length != 0)//check if the frame is empty
	{
		memcpy(refFrame.body,rx_buffer+4, sizeof(int)*refFrame.length);
	}

	memcpy(&(refFrame.tail) , rx_buffer+(sizeof(int)*refFrame.length) + 4, sizeof(int));//copy over the tail after the array
	Print("Receive Frame:");
	Print(FrameInt(refFrame));
}

//sends messages to the connection_socket
void winsock_server::send_message(char * tx_buffer) {
	std::string msg = "Sending: " + (std::string)tx_buffer;
	Print(msg);
	send(this->connection_socket, tx_buffer, strlen(tx_buffer), 0);
}

void winsock_server::send_frame(frame & sendFrame)
{
	char txBuffer[128];

	memcpy(txBuffer, &(sendFrame.length), sizeof(sendFrame.length));//copy over length of array

	if (sendFrame.length != 0)//check if the frame is empty
	{
		memcpy(txBuffer + sizeof(sendFrame.length), sendFrame.body, sizeof(sendFrame.length)*sendFrame.length);
	}

	memcpy(txBuffer + (sizeof(sendFrame.length)*sendFrame.length) + 4, &(sendFrame.tail), sizeof(sendFrame.tail));//copy over the tail after the array
	Print("Sent Frame:");
	Print(FrameString(sendFrame));
	send(this->connection_socket, (char*)txBuffer, (sizeof(int)*sendFrame.length) + emptyFrame, 0);
}

//server constructor that sets up the port number and ip number
//it also initializes the socket, binds it, and puts it in listening mode
winsock_server::winsock_server(int port, std::string ip, std::ofstream* theStream){
	this->ofs = theStream;
	this->port = port;
	this->ip = ip; 
	this->server_socket = this->initialize_tcp_socket();
	this->bind_socket();
	this->listen_socket();
}

//server destructor that closes all sockets opened by this object
winsock_server::~winsock_server(){
	closesocket(this->connection_socket); //closes incoming socket
	closesocket(this->server_socket); //closes server socket
}

//receives messages from the client_socket
char * winsock_client::receive_message() {
	memset(rx_buffer, 0, 128);
	recv(this->client_socket, this->rx_buffer, sizeof(this->rx_buffer), 0);

	std::string msg = "Received: " + (std::string)rx_buffer;
	Print(msg);
	return this->rx_buffer;
}

void winsock_client::receive_frame(frame & refFrame)
{
	recv(this->client_socket, this->rx_buffer, sizeof(this->rx_buffer), 0);

	memcpy(&refFrame.length, rx_buffer, sizeof(int));//copy over length of array
	refFrame.body = new int[refFrame.length];
	if (refFrame.length != 0)//check if the frame is empty
	{
		memcpy(refFrame.body, rx_buffer + 4, sizeof(int)*refFrame.length);
	}

	memcpy(&(refFrame.tail), rx_buffer + (sizeof(int)*refFrame.length) + 4, sizeof(int));//copy over the tail after the array
	Print("Receive Frame:");
	Print(FrameString(refFrame));
}

void winsock_client::send_frame(frame & sendFrame)
{
	char txBuffer[128];
	
	memcpy(txBuffer, &(sendFrame.length), sizeof(sendFrame.length));//copy over length of array
	
	if (sendFrame.length != 0)//check if the frame is empty
	{
		memcpy(txBuffer + sizeof(sendFrame.length), sendFrame.body, sizeof(sendFrame.length)*sendFrame.length);
	}

	memcpy(txBuffer+ (sizeof(sendFrame.length)*sendFrame.length)+4, &(sendFrame.tail), sizeof(sendFrame.tail));//copy over the tail after the array
	Print("Sent Frame:");
	Print(FrameInt(sendFrame));
	send(this->client_socket,(char*) txBuffer, (sizeof(int)*sendFrame.length) + emptyFrame, 0);
}

//sends messages to the client_socket
void winsock_client::send_message(char * tx_buffer) {
	std::string msg = "Sending: " + (std::string)tx_buffer;
	Print(msg);
	send(this->client_socket, tx_buffer, strlen(tx_buffer), 0);
}

//connects to a tcp_server, exits in case the server is unavailable
void winsock_client::connect_to_tcp_server(){
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET; //Address family type internet
	SvrAddr.sin_port = htons(this->port); //port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str()); //IP address
	if ((connect(this->client_socket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(this->client_socket);
		WSACleanup();
		Print("Could not connect to the server");
		std::cin.get();
		exit(0);
	}
}

//connects to a tcp_server, keeps trying at 200ms intervals in case
// the server is unavailable
void winsock_client::connect_to_tcp_server_loop(){
	Print("Trying to connect to the server");

	bool connected = false;
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET; //Address family type internet
	SvrAddr.sin_port = htons(this->port); //port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str()); //IP address
	while (!connected) {
		if ((connect(this->client_socket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		else {
			Print("Connection Established");
			connected = true;
		}
	}
}

//client constructor that sets up the port number and ip number
//it also initializes the socket
winsock_client::winsock_client(int port, std::string ip, std::ofstream *theStream){
	this->ofs = theStream;
	this->port = port;
	this->ip = ip;
	this->client_socket = this->initialize_tcp_socket();
}

//client socket destructor that closes the client_socket
winsock_client::~winsock_client(){
	closesocket(this->client_socket); //closes client socket
}

std::string FrameString(frame & f)
{
	std::string returnString="";
	returnString += "Length: " + std::to_string(f.length) + "\n";

	returnString += "Letters: ";
	for (int i = 0; i < f.length*sizeof(f.length); i++)
	{
		returnString += (char)(*((char*)f.body + i));
	}

	returnString += "\n";
	returnString += "Tail: "+std::to_string(f.tail);
	return returnString;
}

std::string FrameInt(frame & f)
{
	std::string returnString = "";
	returnString += "Length: " + std::to_string(f.length) + "\n";

	returnString += "Numbers: ";
	for (int i = 0; i < f.length; i++)
	{
		returnString += std::to_string(*(f.body + i))+", ";
	}

	returnString += "\n";
	returnString += "Tail: " + std::to_string(f.tail);
	return returnString;
}
