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
#include "intrusiondetect.h"
extern struct PacketInformation packet;
extern struct ids Intrusion;
extern HWND hWnd;
struct TcpHeader DisplayTCP;
struct TCP TcpProtocol;
extern struct IP IpProtocol;
char TcpData[65535];
extern struct IntrusionInfor IntrusionInformation;
struct tcpconnect TcpConnect={0,0,0,0,0,0,0,0,0,0};
int TCPOperation(const unsigned char* packetdata)
{
	Intrusion.protocoltype =1;
	struct TCP* tcp;
	int TcpDataLength;
	unsigned char* data;
	tcp = (struct TCP *) packetdata;
	unsigned short SourcePort;
	SourcePort = ntohs(tcp->SrcPort);
	TcpProtocol.SrcPort =ntohs(tcp->SrcPort) ; 
	unsigned short DestinationPort;
	DestinationPort = ntohs(tcp->DstPort);
	TcpProtocol.DstPort =ntohs(tcp->DstPort) ;
	int HeaderLength;
	HeaderLength = tcp->HdrLen * 4;
	TcpProtocol.HdrLen =tcp->HdrLen ;
	unsigned int  SequenceNum;
	SequenceNum = ntohl(tcp->SequenceNum);
	TcpProtocol.SequenceNum =ntohl(tcp->SequenceNum);
	unsigned int  Acknowledgment;
	Acknowledgment = ntohl(tcp->Acknowledgment);
	TcpProtocol.Acknowledgment = ntohl(tcp->Acknowledgment) ;
	unsigned short AdvertisedWindow;
	AdvertisedWindow = ntohs(tcp->AdvertisedWindow);	
	TcpProtocol.AdvertisedWindow =ntohs(tcp->AdvertisedWindow);
	unsigned short UrgPtr;
	UrgPtr = ntohs(tcp->UrgPtr);
	TcpProtocol.UrgPtr =ntohs(tcp->UrgPtr) ;
	unsigned char Flags;
	Flags = tcp->Flags;
	TcpProtocol.Flags =tcp->Flags ;
	sprintf(DisplayTCP.SrcPort, "%d", SourcePort);
	sprintf(DisplayTCP.DstPort, "%d", DestinationPort);
	strcpy(packet.DestinationPort ,DisplayTCP.DstPort);
	strcpy(packet.SourcePort ,DisplayTCP.SrcPort);
	sprintf(DisplayTCP.SequenceNum, "%u", SequenceNum);
	sprintf(DisplayTCP.Acknowledgment, "%u", Acknowledgment);
	sprintf(DisplayTCP.HdrLen, "%d", HeaderLength);
	sprintf(DisplayTCP.Zero, "%d", tcp->Zero);
	TcpProtocol.Zero =tcp->Zero ;
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
	if ((Flags&0x02)&&(!(Flags&0x10)))
	{
		TcpConnect.SequenceNum1=SequenceNum;
		TcpConnect.ClientIP =IpProtocol.SourceAddr;
		TcpConnect.ServerIP =IpProtocol.DestinationAddr ;
		TcpConnect.ClientPort =tcp->SrcPort;
		TcpConnect.ServerPort=tcp->DstPort ;
		TcpConnect.Connect =false;
		{
			char ipstr[100];
			SOCKADDR_IN addr;
			addr.sin_addr.s_addr=TcpConnect.ClientIP;
			sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
			strcpy(IntrusionInformation.attackip,ipstr);
			strcpy(IntrusionInformation.information ,"SYN标志");
			time_t   currenttime;   
			time(   &currenttime   );   
			sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
			::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
		}
		
	}
	if ((Flags & 0x02)&&(Flags & 0x10))
	{
		if((TcpConnect.ClientIP ==IpProtocol.DestinationAddr)&&
			(TcpConnect.ServerIP ==IpProtocol.SourceAddr)&&
			(TcpConnect.ClientPort ==tcp->DstPort)&&
			(TcpConnect.ServerPort==tcp->SrcPort))
		{
			TcpConnect.Acknowledgment2=Acknowledgment;
			TcpConnect.SequenceNum2=SequenceNum;	
			{
				char ipstr[100];
				SOCKADDR_IN addr;
				addr.sin_addr.s_addr=TcpConnect.ClientIP;
				sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
				strcpy(IntrusionInformation.attackip,ipstr);
				strcpy(IntrusionInformation.information ,"SYN和ACK标志");
				time_t   currenttime;   
				time(   &currenttime   );   
				sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
				::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
			}	
		}
		else
		{
			TcpConnect.Acknowledgment2=0;
			TcpConnect.SequenceNum2=0;	
		}
	}
	if (Flags & 0x10)
	{
		if((TcpConnect.ClientIP ==IpProtocol.SourceAddr)&&
			(TcpConnect.ServerIP ==IpProtocol.DestinationAddr)&&
			(TcpConnect.ClientPort ==tcp->SrcPort)&&
			(TcpConnect.ServerPort==tcp->DstPort))
		{
			TcpConnect.SequenceNum3=SequenceNum;
			TcpConnect.Acknowledgment3=Acknowledgment;
			{
				char ipstr[100];
				SOCKADDR_IN addr;
				addr.sin_addr.s_addr=TcpConnect.ClientIP;
				sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
				strcpy(IntrusionInformation.attackip,ipstr);
				strcpy(IntrusionInformation.information ,"ACK标志");
				time_t   currenttime;   
				time(   &currenttime   );   
				sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
				::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
			}
		}
		else
		{
			TcpConnect.SequenceNum3=0;
			TcpConnect.Acknowledgment3=0;
		}
	}
	if((TcpConnect.Acknowledgment2==TcpConnect.SequenceNum1+1)&&
		(TcpConnect.SequenceNum3==TcpConnect.SequenceNum1+1)&&
		(TcpConnect.Acknowledgment3==TcpConnect.SequenceNum2+1))
	{
		if(!TcpConnect.Connect)
		{
			TcpConnect.Connect =true;
			{
				char ipstr[100];
				SOCKADDR_IN addr;
				addr.sin_addr.s_addr=TcpConnect.ClientIP;
				sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
				strcpy(IntrusionInformation.attackip,ipstr);
				strcpy(IntrusionInformation.information ,"Tcp Connected.");
				time_t   currenttime;   
				time(   &currenttime   );   
				sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
				::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
			}
		}
	}
	sprintf(DisplayTCP.Flags, "%s", myflags);
	sprintf(DisplayTCP.AdvertisedWindow, "%d", AdvertisedWindow);
	sprintf(DisplayTCP.Checksum, "%u", ntohs(tcp->Checksum));
	TcpProtocol.Checksum =ntohs(tcp->Checksum) ;
	sprintf(DisplayTCP.UrgPtr, "%d", UrgPtr);
	strcpy(TcpData, "");
	TcpDataLength=IpProtocol.Length-IpProtocol.HeaderLength*4-tcp->HdrLen*4;
	data = (unsigned char *) packetdata;
	data =data+ (tcp->HdrLen)*4;
	if (TcpDataLength > 0)
	{
		for (int i = 0; i < TcpDataLength; i++)
		{
			
			if (isgraph(data[i]))
				TcpData[i] = data[i];
			else if (data[i] == ' ')
				TcpData[i]= data[i];
			else
				TcpData[i] = '.';	
		}
		TcpData[i]='\0';
	}
	::PostMessage(hWnd, WM_MY_MESSAGE_TCP, 0, 0);
	return 0;
}
