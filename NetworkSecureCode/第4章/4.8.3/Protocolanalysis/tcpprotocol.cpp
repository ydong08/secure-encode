//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include "sniffer.h"
#include <stdio.h>
#include "tcpprotocol.h"
#include "ipprotocol.h"
extern struct PacketInformation packet;
extern HWND hWnd;
struct TcpHeader DisplayTCP; 
int TCPOperation(const unsigned char* packetdata)
{
	struct TCP* tcp;
	tcp = (struct TCP *) packetdata;
	unsigned short SourcePort;
	SourcePort = ntohs(tcp->SrcPort);
	unsigned short DestinationPort;
	DestinationPort = ntohs(tcp->DstPort);
	int HeaderLength;
	HeaderLength = tcp->HdrLen * 4;
	unsigned int  SequenceNum;
	SequenceNum = ntohl(tcp->SequenceNum);
	unsigned int  Acknowledgment;
	Acknowledgment = ntohl(tcp->Acknowledgment);
	unsigned short AdvertisedWindow;
	AdvertisedWindow = ntohs(tcp->AdvertisedWindow);	
	unsigned short UrgPtr;
	UrgPtr = ntohs(tcp->UrgPtr);
	unsigned char Flags;
	Flags = tcp->Flags;
	sprintf(DisplayTCP.SrcPort, "%d", SourcePort);
	sprintf(DisplayTCP.DstPort, "%d", DestinationPort);
	strcpy(packet.DestinationPort ,DisplayTCP.DstPort);
	strcpy(packet.SourcePort ,DisplayTCP.SrcPort);
	sprintf(DisplayTCP.SequenceNum, "%u", SequenceNum);
	sprintf(DisplayTCP.Acknowledgment, "%u", Acknowledgment);
	sprintf(DisplayTCP.HdrLen, "%d", HeaderLength);
	sprintf(DisplayTCP.Zero, "%d", tcp->Zero);
	char myflags[1024];
	strcpy(myflags, "");
	if (Flags & 0x02)
	{
		strcat(myflags, "SYN ");
	}
	if (Flags & 0x01)
	{
		strcat(myflags, "FIN ");
	}
	if (Flags & 0x04)
	{
		strcat(myflags, "RST ");
	}
	if (Flags & 0x08)
	{
		strcat(myflags, "PSH ");
	}
	if (Flags & 0x10)
	{
		strcat(myflags, "ACK ");
	}
	if (Flags & 0x20)
	{
		strcat(myflags, "URG ");
	}
	sprintf(DisplayTCP.Flags, "%s", myflags);
	sprintf(DisplayTCP.AdvertisedWindow, "%d", AdvertisedWindow);
	sprintf(DisplayTCP.Checksum, "%d", ntohs(tcp->Checksum));
	sprintf(DisplayTCP.UrgPtr, "%d", UrgPtr);
	::PostMessage(hWnd, WM_MY_MESSAGE_TCP, 0, 0);
	return 0;
}
