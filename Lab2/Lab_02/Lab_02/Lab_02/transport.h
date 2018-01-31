#ifndef ETHERNET_H
#define ETHERNET_H
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

struct udp_packet{
	int source_port, dest_port;
	string data;
};

struct tcp_packet {
	int source_port, dest_port, seq_number; bool acknowledge, checksum;
	string data;
};

class channel {
private:
	int prob_lost_data, prob_corrupt_data;
public:
	channel(int, int);
	tcp_packet transmit(tcp_packet);
	udp_packet transmit(udp_packet);
};


class udp_node {
protected:
	int current_index, data_size, port;
	string data;
public:
	udp_packet rec;
	udp_packet send_packet(int);
	int get_data_size() { return this->data_size; }
	int get_current_index() { return this->current_index; }
	void update_current_index() { this->current_index++; }
	void request_transmission(udp_node *transmitter, channel * wireless);
	void print_data(ofstream &os) { os << this->data << endl; }
};

class udp_server : public udp_node {
public:
	udp_server(int, string);
};

class udp_client : public udp_node {
public:
	udp_client(int);
};


class tcp_node {
protected:
	string data;
	int port, current_index, data_size;
public:
	tcp_packet rec;
	tcp_packet send_packet(int);
	int get_data_size() { return this->data_size; }
	int get_current_index() { return this->current_index; }
	void update_current_index() { this->current_index++; }
	void request_transmission(tcp_node *transmitter, channel * wireless);
	void send_signal(tcp_node *dest, tcp_node *transmitter, channel *wireless, string data);
	void print_data(ofstream&) { cout << this->data << endl; }
	bool three_way_handshake(tcp_node *, channel *);
	bool four_way_handshake(tcp_node *, channel *);
};
class tcp_server : public tcp_node {
public:
	tcp_server(int, string);
};
class tcp_client : public tcp_node {
public:
	tcp_client(int);
};

#endif
