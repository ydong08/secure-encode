#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include "rawsocsniffer.h"
#include "inetheader.h"

using namespace std;

rawsocsniffer::rawsocsniffer(int protocol):rawsocket(protocol),max_packet_len(2048)
{
    packet=new char[max_packet_len];
    memset(&simfilter,0,sizeof(simfilter));
}

rawsocsniffer::~rawsocsniffer()
{
    delete[] packet;
}

//set the socket to promiscuous mode which means to capture all packets.
bool rawsocsniffer::init()
{
    dopromisc("eth0");
}

//set the filter
void rawsocsniffer::setfilter(filter myfilter)
{
    simfilter.protocol=myfilter.protocol;
    simfilter.sip=myfilter.sip;
    simfilter.dip=myfilter.dip;
}

bool rawsocsniffer::testbit(const unsigned int p,int k)
{
    if((p>>(k-1))&0x01)
	return true;
    else
	return false;
}

void rawsocsniffer::setbit(unsigned int &p,int k)
{
    p=(p)|((0x01)<<(k-1));
}

//capture packets;
void rawsocsniffer::sniffer()
{
    struct sockaddr_in from;
    int sockaddr_len=sizeof(struct sockaddr_in);
    int recvlen=0;
    while(1)
    {
    	recvlen=receive(packet,max_packet_len,&from,&sockaddr_len);
    	if(recvlen>0)
    	{
	    analyze();
    	}
   	 else
    	{
	    continue;
    	}
    }	 
}

//analyze packets;
void rawsocsniffer::analyze()
{
    ether_header_t *etherpacket=(ether_header_t *)packet;
    if(simfilter.protocol==0)
	simfilter.protocol=0xff;
    switch (ntohs(etherpacket->frametype))
    {
	case 0x0800:
	    if(((simfilter.protocol)>>1))
	    {
	    	cout<<"\n\n/*---------------ip packet--------------------*/"<<endl;
	    	ParseIPPacket();
	    }
	    break;
	case 0x0806:
	    if(testbit(simfilter.protocol,1))
	    {
	    	cout<<"\n\n/*--------------arp packet--------------------*/"<<endl;
	    	ParseARPPacket();
	    }
	    break;
	case 0x0835:
	    if(testbit(simfilter.protocol,5))
	    {
		cout<<"\n\n/*--------------RARP packet--------------------*/"<<endl;
		ParseRARPPacket();
	    }
	    break;
	default:
	    cout<<"\n\n/*--------------Unknown packet----------------*/"<<endl;
	    cout<<"Unknown ethernet frametype!"<<endl;
	    break;
    }
}

//analyze ip packets
void rawsocsniffer::ParseIPPacket()
{
    ip_packet_t *ippacket=(ip_packet_t *)packet; 
    cout<<"ipheader.protocol: "<<int(ippacket->ipheader.protocol)<<endl;
    if(simfilter.sip!=0)
    {
	if(simfilter.sip!=(ippacket->ipheader.src_ip))
	    return;
    }

    if(simfilter.dip!=0)
    {
	if(simfilter.dip!=(ippacket->ipheader.des_ip))
	    return;
    }

    switch (int(ippacket->ipheader.protocol))
    {
	case 1:
	    if(testbit(simfilter.protocol,4))
	    {
	    	cout<<"Received an ICMP packet"<<endl;
	    	ParseICMPPacket();
	    }
	    break;
	case 6:
	    if(testbit(simfilter.protocol,2))
	    {
	    	cout<<"Received an TCP packet"<<endl;
	    	ParseTCPPacket();
	    }
	    break;
	case 17:
	    if(testbit(simfilter.protocol,3))
	    {
	    	cout<<"Received an UDP packet"<<endl;
	    	ParseUDPPacket();
	    }
	    break;
	default:
	    cout<<"Unknown ip protocoltype"<<endl;
	    break;
    }
    
}

//analyze RARP packets;

void rawsocsniffer::ParseRARPPacket()
{

}


//analyze arp packets;
void rawsocsniffer::ParseARPPacket()
{
    arp_packet_t *arppacket=(arp_packet_t *)packet;
    print_hw_addr(arppacket->arpheader.send_hw_addr);
    print_hw_addr(arppacket->arpheader.des_hw_addr);
    cout<<endl;
    print_ip_addr(arppacket->arpheader.send_prot_addr);
    print_ip_addr(arppacket->arpheader.des_prot_addr);
    cout<<endl;
    cout<<setw(15)<<"Hardware type: "<<"0x"<<hex<<ntohs(arppacket->arpheader.hw_type);
    cout<<setw(15)<<"  Protocol type: "<<"0x"<<hex<<ntohs(arppacket->arpheader.prot_type);
    cout<<setw(15)<<"  Operation code: "<<"0x"<<hex<<ntohs(arppacket->arpheader.flag);
    cout<<endl;
}

//analyze udp packets;
void rawsocsniffer::ParseUDPPacket()
{
    udp_packet_t *udppacket=(udp_packet_t *)packet;
    cout<<setw(20)<<"MAC address: from ";
    print_hw_addr(udppacket->etherheader.src_hw_addr);
    cout<<"to ";
    print_hw_addr(udppacket->etherheader.des_hw_addr);
    cout<<endl<<setw(20)<<"IP address: from ";
    print_ip_addr(udppacket->ipheader.src_ip);
    cout<<"to ";
    print_ip_addr(udppacket->ipheader.des_ip);
    cout<<endl;
    cout<<setw(10)<<"srcport: "<<ntohs(udppacket->udpheader.src_port)<<" desport: "<<ntohs(udppacket->udpheader.des_port)\
	<<" length:"<<ntohs(udppacket->udpheader.len)<<endl;
}

//analyze tcp packets;
void rawsocsniffer::ParseTCPPacket()
{
    tcp_packet_t *tcppacket=(tcp_packet_t *)packet;
    cout<<setw(20)<<"MAC address: from ";
    print_hw_addr(tcppacket->etherheader.src_hw_addr);
    cout<<"to ";
    print_hw_addr(tcppacket->etherheader.des_hw_addr);
    cout<<endl<<setw(20)<<"IP address: from ";
    print_ip_addr(tcppacket->ipheader.src_ip);
    cout<<"to ";
    print_ip_addr(tcppacket->ipheader.des_ip);
    cout<<endl;
    cout<<setw(10)<<"srcport: "<<ntohs(tcppacket->tcpheader.src_port)<<" desport: "<<ntohs(tcppacket->tcpheader.des_port)<<endl;
    cout<<"seq: "<<ntohl(tcppacket->tcpheader.seq)<<" ack: "<<ntohl(tcppacket->tcpheader.ack)<<endl;
}

//analyze icmp packets;
void rawsocsniffer::ParseICMPPacket()
{
    icmp_packet_t *icmppacket=(icmp_packet_t *)packet;
    cout<<setw(20)<<"MAC address: from ";
    print_hw_addr(icmppacket->etherheader.src_hw_addr);
    cout<<"to ";
    print_hw_addr(icmppacket->etherheader.des_hw_addr);
    cout<<endl<<setw(20)<<"IP address: from ";
    print_ip_addr(icmppacket->ipheader.src_ip);
    cout<<"to ";
    print_ip_addr(icmppacket->ipheader.des_ip);
    cout<<endl;
    cout<<setw(12)<<"icmp type: "<<int(icmppacket->icmpheader.type)<<" icmp code: "<<int(icmppacket->icmpheader.code)<<endl;
    cout<<setw(12)<<"icmp id: "<<ntohs(icmppacket->icmpheader.id)<<" icmp seq: "<<ntohs(icmppacket->icmpheader.seq)<<endl;
}

void rawsocsniffer::print_hw_addr(const unsigned char *ptr)
{
    char hw_addr[18];
    sprintf(hw_addr,"%02x:%02x:%02x:%02x:%02x:%02x",ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);
    cout<<setiosflags(ios::left)<<setw(20)<<hw_addr;
}

void rawsocsniffer::print_ip_addr(const unsigned long ip)
{
    cout<<setiosflags(ios::left)<<setw(18)<<inet_ntoa(*(in_addr *)&(ip));
}
