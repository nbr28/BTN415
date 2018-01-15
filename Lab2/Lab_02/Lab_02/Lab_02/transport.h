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

class channel {
private:
	int prob_lost_data, prob_corrupt_data;
public:
	channel(int, int);
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

#endif
