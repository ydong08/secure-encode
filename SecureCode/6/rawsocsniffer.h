#ifndef _RAWSOCSNIFFER_H
#define _RAWSOCSNIFFER_H

#include "rawsocket.h"

//filter structure, you can define your own filter struct here;
typedef struct filter
{
    unsigned long sip;
    unsigned long dip;
    unsigned int protocol;    
} filter;

class rawsocsniffer:public rawsocket
{
    private:
	filter simfilter;
	char *packet;
	const int max_packet_len;
    public:
	rawsocsniffer(int protocol);
	~rawsocsniffer();
	bool init();
	void setfilter(filter myfilter);
	bool testbit(const unsigned int p, int k);
	void setbit(unsigned int &p,int k);
	void sniffer();
	void analyze();
	void ParseRARPPacket();
	void ParseARPPacket();
	void ParseIPPacket();
	void ParseTCPPacket();
	void ParseUDPPacket();
	void ParseICMPPacket();
	void print_hw_addr(const unsigned char *ptr);
	void print_ip_addr(const unsigned long ip);
};

#endif
