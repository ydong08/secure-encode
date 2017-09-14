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
#include "intrusiondetect.h"
extern HWND hWnd;
struct UdpHeader DisplayUDP;
extern struct PacketInformation packet;
extern struct ids Intrusion;
struct UDP UdpProtocol;
char UdpData[65535];
int UDPOperation(const unsigned char* packetdata)
{
	Intrusion.protocoltype =2;
	struct UDP* udp;
	int UdpDataLength;
	unsigned char* data;
	udp = (struct UDP *) packetdata;
	unsigned short SourcePort; 
	SourcePort = ntohs(udp->SrcPort);
	UdpProtocol.SrcPort =ntohs(udp->SrcPort) ;
	unsigned short DestinationPort; 
	DestinationPort = ntohs(udp->DstPort);
	UdpProtocol.DstPort =ntohs(udp->DstPort );
	unsigned short Length;
	Length = ntohs(udp->Length);
	UdpProtocol.Length =ntohs(udp->Length );
	sprintf(DisplayUDP.SrcPort, "%d", SourcePort);
	sprintf(DisplayUDP.DstPort, "%d", DestinationPort);
	strcpy(packet.DestinationPort ,DisplayUDP.DstPort);
	strcpy(packet.SourcePort ,DisplayUDP.SrcPort);
	sprintf(DisplayUDP.Length, "%d", Length);
	sprintf(DisplayUDP.Checksum, "%u", ntohs(udp->Checksum));
	UdpProtocol.Checksum =ntohs(udp->Checksum );
	strcpy(UdpData, "");
	UdpDataLength=Length-8;
	TRACE("TcpDataLength:%d\n",UdpDataLength);
	data = (unsigned char *) packetdata;
	data =data+ 8;
	if (UdpDataLength > 0)
	{
		for (int i = 0; i < UdpDataLength; i++)
		{
			if (isgraph(data[i]))
				UdpData[i] = data[i];
			else if (data[i] == ' ')
				 UdpData[i]= data[i];
			else
				UdpData[i] = '.';	
		}
		UdpData[i]='\0';
	}
	TRACE("\n");
	TRACE(UdpData);
	::PostMessage(hWnd, WM_MY_MESSAGE_UDP, 0, 0);
	return 0;
}
