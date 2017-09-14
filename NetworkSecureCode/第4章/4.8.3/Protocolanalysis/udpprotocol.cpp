//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include <stdio.h>
#include "udpprotocol.h"
#include "ipprotocol.h"
#include "sniffer.h"
extern HWND hWnd;
struct UdpHeader DisplayUDP;
extern struct PacketInformation packet;
int UDPOperation(const unsigned char* packetdata)
{
	struct UDP* udp;
	udp = (struct UDP *) packetdata;
	unsigned short SourcePort; 
	SourcePort = ntohs(udp->SrcPort);	
	unsigned short DestinationPort; 
	DestinationPort = ntohs(udp->DstPort); 
	unsigned short Length;
	Length = ntohs(udp->Length);
	sprintf(DisplayUDP.SrcPort, "%d", SourcePort);
	sprintf(DisplayUDP.DstPort, "%d", DestinationPort);
	strcpy(packet.DestinationPort ,DisplayUDP.DstPort);
	strcpy(packet.SourcePort ,DisplayUDP.SrcPort);
	sprintf(DisplayUDP.Length, "%d", Length);
	sprintf(DisplayUDP.Checksum, "%d", ntohs(udp->Checksum));
	::PostMessage(hWnd, WM_MY_MESSAGE_UDP, 0, 0);
	return 0;
}
