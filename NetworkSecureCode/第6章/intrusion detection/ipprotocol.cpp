//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include <stdio.h>
#include <sys/types.h>
#include "ipprotocol.h"
#include "tcpprotocol.h"
#include "icmpprotocol.h"
#include "udpprotocol.h"
#include "sniffer.h"
extern HWND hWnd;
extern struct PacketInformation packet;
struct IpHeader DisplayIP;
struct IP IpProtocol;
void IPOperation(const unsigned char* packetdata)
{
	struct IP* ip;
	unsigned int  HeaderLength;
	unsigned int 	Length;
	unsigned int 	off;
	unsigned char* IPOver = NULL;
	unsigned int Off;
	unsigned char Tos;
	unsigned short Checksum;
	ip = (struct IP *) packetdata;
	Length = ntohs(ip->Length);
	IpProtocol.Length =ntohs(ip->Length);
	Checksum = ntohs(ip->Checksum);
	IpProtocol.Checksum =ntohs(ip->Checksum );
	HeaderLength = ip->HeaderLength * 4;
	IpProtocol.HeaderLength =ip->HeaderLength;  
	sprintf(DisplayIP.Version, "%d", ip->Version);
	IpProtocol.Version =ip->Version ;
	sprintf(DisplayIP.HeaderLength, "%d", HeaderLength);
	Tos = ip->Tos;
	IpProtocol.Tos =ip->Tos ;
	sprintf(DisplayIP.Tos, "%d", Tos);
	sprintf(DisplayIP.Length, "%d", ntohs(ip->Length));
	sprintf(DisplayIP.Ident, "%d", ntohs(ip->Ident));
	IpProtocol.Ident =ntohs(ip->Ident );
	Off = ntohs(ip->Flags_Offset);
	IpProtocol.Flags_Offset =ntohs(ip->Flags_Offset) ;
	sprintf(DisplayIP.Flags_Offset, "%d", (Off & 0x1fff) * 8);
	sprintf(DisplayIP.TTL, "%d", ip->TTL);
	IpProtocol.TTL =ip->TTL ;
	sprintf(DisplayIP.Protocol, "%d", ip->Protocol);
	IpProtocol.Protocol =ip->Protocol ;
	sprintf(DisplayIP.Checksum, "%d", Checksum);
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr=ip->SourceAddr;
	sprintf(DisplayIP.SourceAddr, "%s", inet_ntoa(addr.sin_addr));
	IpProtocol.SourceAddr =ip->SourceAddr ;
	addr.sin_addr.s_addr=ip->DestinationAddr;
	sprintf(DisplayIP.DestinationAddr, "%s", inet_ntoa(addr.sin_addr));
	IpProtocol.DestinationAddr =ip->DestinationAddr ;
	strcpy(packet.SourceAddr ,DisplayIP.SourceAddr);
	strcpy(packet.DestinationAddr ,DisplayIP.DestinationAddr);	
	::PostMessage(hWnd, WM_MY_MESSAGE_IP, 0, 0);
	Length =Length-HeaderLength;
	off = ntohs(ip->Flags_Offset);
	if ((off & 0x1fff) == 0)
	{
		IPOver = (unsigned char *) ip + HeaderLength;
		switch (ip->Protocol)
		{
		case 6:
			sprintf(packet.NetType, "%s", "TCP");
			TCPOperation(IPOver);
			break;
		case 17:
			sprintf(packet.NetType, "%s", "UDP");
			UDPOperation(IPOver);
			break;
		case 1:
			sprintf(packet.NetType, "%s", "ICMP");
			ICMPOperation(IPOver);
			break;
		default:
			break;
		}
	}
}
