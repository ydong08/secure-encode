//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//


#include "stdafx.h"
#include "winsock2.h"
#include "intrusiondetect.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "icmpprotocol.h"
#include "ipprotocol.h"
#include "tcpprotocol.h"
#include "udpprotocol.h"
#include "ProtocolanalysisDlg.h"
struct ids Intrusion;
struct IntrusionInfor IntrusionInformation;
extern struct ICMP IcmpProtocol;
extern struct IP IpProtocol;
extern struct TCP TcpProtocol;
extern struct UDP UdpProtocol;
extern HWND hWnd;
extern char TcpData[65535];
extern tcpconnect TcpConnect;
char * BM(char *MainString, char *SubString,bool UpperLower) 
{ 	
	char *SubRear=NULL; 
	char *MainRear=NULL; 
	char *SubIndex=NULL; 
	char *Rear=NULL;
	int SubLength=0;
	int MainLength=0;
	int Count; 
	char *MainStr=NULL;
	char *SubStr=NULL;
	char * Position=NULL;
	char *OldIndex=NULL;
	char *MainString2=(char *)malloc(strlen(MainString));
	strcpy(MainString2,MainString);
	char *SubString2=(char *)malloc(strlen(SubString));
	strcpy(SubString2,SubString);
	if(!UpperLower)
	{
		MainStr=strupr(MainString2);
		SubStr=strupr(SubString2);
	}
	else
	{
		MainStr=MainString;
		SubStr=SubString;
	}
	Position= MainStr; 
	OldIndex=MainString;
	SubLength   = strlen(SubStr); 
	MainLength   = strlen(MainStr); 
	SubRear   = SubStr+SubLength-1; 
	MainRear   = MainStr+MainLength-1; 
	while (Position<=MainRear) 
	{ 
		SubIndex = SubRear; 
		while (SubIndex>=SubStr)
		{ 
			if (*Position!=*SubIndex ) 
			{ 
				Rear = strrchr(SubStr,*Position ); 
				if ( Rear==NULL) 
					Count = SubLength; 
				else 
					Count = SubRear - Rear; 
				break; 
			} 
			SubIndex --; 
			Position --; 
			OldIndex--;
		} 
		if (SubIndex<SubStr) 
		{
			return(OldIndex+1);  
		}
		Position=Position+Count; 
		OldIndex=OldIndex+Count;
	} 
	return NULL; 
}
int WebServerIntrusionDetect(void)
{
	if(BM(TcpData,"cmd.exe",false)!=NULL)
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"cmd.exe命令操作");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	if(BM(TcpData,"cgi-bin/phf",false)!=NULL)
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"CGI-PHF 尝试");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	//...
	return 1;
}
int EmailServerIntrusionDetect()
{
	if((BM(TcpData,"EXPN",false)!=NULL)&&(BM(TcpData,"DECODE",false)!=NULL))
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"EXPN DECODE 尝试");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	return 1;
}

int FtpServerIntrusionDetect()
{
	if((BM(TcpData,"SITE",false)!=NULL)&&(BM(TcpData,"EXEC",false)!=NULL))
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"CGI-PHF 尝试");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	return 1;
}
int Pop3ServerIntrusionDetect()
{
	if(BM(TcpData,"PASS",false)!=NULL)
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"PASS 尝试");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	return 1;
}

int TcpIntrusionDetect(void)
{
	
	if((TcpProtocol.DstPort==80)||(TcpProtocol.SrcPort==80))
	{
		WebServerIntrusionDetect();
	}
	if(TcpProtocol.DstPort==25)
	{
		EmailServerIntrusionDetect();
	}
	if(TcpProtocol.DstPort==21)
	{
		FtpServerIntrusionDetect();
	}
	if(TcpProtocol.DstPort==110)
	{
		Pop3ServerIntrusionDetect();
	}
	return 1;
}

int UdpIntrusionDetect(void)
{
	
	return 1;
}
int IcmpIntrusionDetect(void)
{
	if(IcmpProtocol.Type ==8)
	{
		char ipstr[100];
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr=IpProtocol.SourceAddr;
		sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
		strcpy(IntrusionInformation.attackip,ipstr);
		strcpy(IntrusionInformation.information ,"Ping Scan.");
		time_t   currenttime;   
        time(   &currenttime   );   
        sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
		::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
	}
	return 1;
}
UINT SYNScanDetectThread(LPVOID pParam)
{
			HANDLE Timer = NULL;
			LARGE_INTEGER TimerNumber;
			TimerNumber.QuadPart = -50000000;
			Timer =CreateWaitableTimer(NULL, TRUE, "NewTimer");
			if (Timer==NULL)
			{           
				AfxMessageBox("CreateWaitableTimer with error");
				return 0;
			}
			if (!SetWaitableTimer(Timer, &TimerNumber, 0, NULL, NULL, 0))
			{         
				AfxMessageBox("SetWaitableTimer with error");
				CloseHandle(Timer);
				return 0;
			}
			if (WaitForSingleObject(Timer, INFINITE) != WAIT_OBJECT_0)
			{ 
				AfxMessageBox("WaitForSingleObject with error");
				CloseHandle(Timer);
				return 0;
			}
			else
			{
				if(!(TcpConnect.Connect))
				{
					char ipstr[100];
					SOCKADDR_IN addr;
					addr.sin_addr.s_addr=TcpConnect.ClientIP;
					sprintf(ipstr, "%s", inet_ntoa(addr.sin_addr));
					strcpy(IntrusionInformation.attackip,ipstr);
					strcpy(IntrusionInformation.information ,"SYN Scan.");
					time_t   currenttime;   
					time(   &currenttime   );   
					sprintf(IntrusionInformation.time,"%s",ctime(&currenttime));  
					::PostMessage(hWnd, WM_MY_MESSAGE_INTRUSION, 0, 0);
				}
			}
			CloseHandle(Timer);
	return 0;

}
int ScanDetect(void)
{
		if((TcpProtocol.Flags &0x02)&&(!(TcpProtocol.Flags &0x10)))
		{
			AfxBeginThread(SYNScanDetectThread, hWnd);
		}
	return 1;

}
int IntrusionDetect(void)
{
    switch (Intrusion.protocoltype)
	{
	case 1:
		TcpIntrusionDetect();
		ScanDetect();
		break;
	case 2:
		UdpIntrusionDetect();
		break;
	case 3:
		IcmpIntrusionDetect();
		break;
	default:
		break;
	}
	return 1;
}