
//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include <stdio.h>
#include "sniffer.h"
#include "arpprotocol.h"
struct arpheader DisplayARP;
extern HWND hWnd;
void ARPOperation(const unsigned char* packetdata)
{
	unsigned short Protocol;
	unsigned short Hardware;
	unsigned short Operation;
	struct ARP *Arp;
	sprintf(DisplayARP.HardwareLength, "%s", "");
	sprintf(DisplayARP.Hardware, "%s", "");
	sprintf(DisplayARP.OperationCode, "%s", "");
	sprintf(DisplayARP.ProtocolLength, "%s", "");
	sprintf(DisplayARP.Protocol, "%s", "");
	sprintf(DisplayARP.OperationInformatin, "%s", "");
	Arp = (struct ARP *) packetdata;
	Hardware = ntohs(Arp->Hardware);
	Protocol = ntohs(Arp->Protocol);
	Operation = ntohs(Arp->OperationCode);
	sprintf(DisplayARP.HardwareLength, "%d", ntohs(Arp->HardwareLength));
	sprintf(DisplayARP.ProtocolLength, "%d", ntohs(Arp->ProtocolLength));
	sprintf(DisplayARP.Hardware, "%d", Hardware);
	sprintf(DisplayARP.Protocol, "%d", Protocol);
	sprintf(DisplayARP.OperationCode, "%d", Operation);
	switch (Operation)
	{
	case 1:
		sprintf(DisplayARP.OperationInformatin, "%s", "ARP请求");
		::PostMessage(hWnd, WM_MY_MESSAGE_ARP, 0, 0);
		break;
	case 2:
		sprintf(DisplayARP.OperationInformatin, "%s", "ARP应答");
		::PostMessage(hWnd, WM_MY_MESSAGE_ARP, 0, 0);
		break;
	case 3:
		printf("(RARP request)\n");
		sprintf(DisplayARP.OperationInformatin, "%s", "RARP请求");
		::PostMessage(hWnd, WM_MY_MESSAGE_ARP, 0, 0);
		break;
	case 4:
		sprintf(DisplayARP.OperationInformatin, "%s", "RARP应答");
		::PostMessage(hWnd, WM_MY_MESSAGE_ARP, 0, 0);
		break;
	default:
		sprintf(DisplayARP.OperationInformatin, "%s", "未知");
		::PostMessage(hWnd, WM_MY_MESSAGE_ARP, 0, 0);
		return ;
	}
}
