//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include <stdio.h>
#include <sys/types.h>
#include "icmpprotocol.h"
#include "sniffer.h"
#include "intrusiondetect.h"
struct IcmpHeader DisplayIcmp;	
extern struct ids Intrusion;
struct ICMP IcmpProtocol;
extern HWND hWnd;
void ICMPOperation(const unsigned char* packetdata)
{
	Intrusion.protocoltype =3;
	struct ICMP* Icmp;
	Icmp = (struct ICMP *) packetdata;
	sprintf(DisplayIcmp.type, "%d", Icmp->Type);
	IcmpProtocol.Type =Icmp->Type;
	switch (Icmp->Type)
	{
	case 0:
		sprintf(DisplayIcmp.information, "%s", "回显应答");
		break;
	case 8:
		sprintf(DisplayIcmp.information, "%s", "回显请求");
		break;
	case 13:
		sprintf(DisplayIcmp.information, "%s", "时间戳请求");
		break;
	case 14:
		sprintf(DisplayIcmp.information, "%s", "时间戳应答");
		break;
	case 17:
		sprintf(DisplayIcmp.information, "%s", "地址掩码请求");
		break;
	case 18:
		sprintf(DisplayIcmp.information, "%s", "地址掩码应答");
		break;
	default:
		sprintf(DisplayIcmp.information, "%s", "ICMP类型未知");
		break;
	}
	sprintf(DisplayIcmp.code, "%d", Icmp->Code);
	IcmpProtocol.Code =Icmp->Code ;
	sprintf(DisplayIcmp.checksum, "%d", ntohs(Icmp->Checksum));
	IcmpProtocol.Checksum =ntohs(Icmp->Checksum); 
	::PostMessage(hWnd, WM_MY_MESSAGE_ICMP, 0, 0);
	return;
}

