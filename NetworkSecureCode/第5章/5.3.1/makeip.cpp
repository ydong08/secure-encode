//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h" 
#include "string.h"
#include "Winsock2.h"
#include <ws2tcpip.h>    
#include "mstcpip.h"
#pragma comment(lib,"WS2_32.lib") 
struct IpHeader
{
    unsigned char Version_HLen;
    unsigned char TOS;
    unsigned short Length;
    unsigned short Ident;
    unsigned short Flags_Offset;
    unsigned char TTL;
    unsigned char Protocol;
    unsigned short Checksum;
    unsigned int SourceAddr;
    unsigned int DestinationAddr;
};
unsigned short checksum(unsigned short *data, int length)
{
    unsigned long temp = 0;
    while (length > 1)
    {
        temp +=  *data++;
        length -= sizeof(unsigned short);
    }
    if (length)
    {
        temp += *(unsigned short*)data;
    }
    temp = (temp >> 16) + (temp &0xffff);
    temp += (temp >> 16);
    return (unsigned short)(~temp);
}
int main(void)
{
    char *DestIpAddr = "192.168.1.3";
    struct in_addr localaddr;
    char HostName[255];
    struct hostent *Hostent;
    WSADATA wsaData;
    SOCKET SendSocket;
    SOCKADDR_IN addr_in;
    struct IpHeader ipHeader;
    char szSendBuf[1000] = 
    {
        0
    };
    char IpData[100] = "cdefg";
    BOOL flag;
    int nTimeOver;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    if ((SendSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("WSASocket failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    flag = true;
    if (setsockopt(SendSocket, IPPROTO_IP, IP_HDRINCL, (char*) &flag, sizeof(flag)) == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    nTimeOver = 1000;
    if (setsockopt(SendSocket, SOL_SOCKET, SO_SNDTIMEO, (char*) &nTimeOver, sizeof(nTimeOver)) == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(1000);
    addr_in.sin_addr.S_un.S_addr = inet_addr(DestIpAddr);
    Result = gethostname(HostName, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    }
    Hostent = (struct hostent*)malloc(sizeof(struct hostent));
    Hostent = gethostbyname(HostName);
    memcpy(&localaddr, Hostent->h_addr_list[0], Hostent->h_length);
    ipHeader.Version_HLen = (4 << 4 | sizeof(ipHeader) / sizeof(unsigned long));
    ipHeader.TOS = 0;
    ipHeader.Length = htons(sizeof(ipHeader) + strlen(IpData));
    ipHeader.Ident = htons(1);
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = 27;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = localaddr.S_un.S_addr;
    ipHeader.DestinationAddr = inet_addr(DestIpAddr);
    memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
    memcpy(szSendBuf + sizeof(ipHeader), IpData, sizeof(IpData));
    Result = sendto(SendSocket, szSendBuf, sizeof(ipHeader) + strlen(IpData), 0, (struct sockaddr*) &addr_in, sizeof(addr_in));
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("fff%d\n", sizeof(ipHeader));
        printf("Send Ip Packet OK.\n");
        printf("Version:%d\n", ipHeader.Version_HLen >> 4);
        printf("Herader Length:%d\n", (ipHeader.Version_HLen &0x0f) *4);
        printf("TOS:%d\n", ipHeader.TOS);
        printf("Length:%d\n", ntohs(ipHeader.Length));
        printf("Ident:%d\n", ntohs(ipHeader.Ident));
        printf("Flags_Offset:%d\n", ntohs(ipHeader.Flags_Offset));
        printf("TTL:%d\n", ipHeader.TTL);
        printf("Protocol:%d\n", ipHeader.Protocol);
        printf("Checksum:%u\n", ntohs(ipHeader.Checksum));
        SOCKADDR_IN ip;
        ip.sin_addr.s_addr = ipHeader.SourceAddr;
        printf("Source IP:%s\n", inet_ntoa(ip.sin_addr));
        ip.sin_addr.s_addr = ipHeader.DestinationAddr;
        printf("Destination IP:%s\n", inet_ntoa(ip.sin_addr));
    }
    if (closesocket(SendSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 0;
    }
    return 1;
}
