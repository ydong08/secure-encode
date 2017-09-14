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
struct IcmpHeader
{
    unsigned char Type;
    unsigned char Code;
    unsigned short Checksum;
    unsigned short ID;
    unsigned short Sequence;
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

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    sockaddr_in DestAddr;
    struct IpHeader ipHeader;
    struct IcmpHeader SendIcmp;
    int Timeout = 1000;
    char DestIpAddr[100] = "192.168.1.3";
    char IcmpBuffer[100] = "";
    char IcmpData[] = "Here is ICMP data content.";
    int flag = true;
    SOCKET IcmpSocket;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    } if ((IcmpSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("WSASocket failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    if (setsockopt(IcmpSocket, IPPROTO_IP, IP_HDRINCL, (char*) &flag, sizeof(flag)) == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    if (setsockopt(IcmpSocket, SOL_SOCKET, SO_SNDTIMEO, (char*) &Timeout, sizeof(Timeout)) == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    memset(&DestAddr, 0, sizeof(DestAddr));
    DestAddr.sin_addr.s_addr = inet_addr(DestIpAddr);
    DestAddr.sin_port = htons(0);
    DestAddr.sin_family = AF_INET;
    struct in_addr localaddr;
    char HostName[255];
    struct hostent *Hostent;
    Result = gethostname(HostName, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } Hostent = (struct hostent*)malloc(sizeof(struct hostent));
    Hostent = gethostbyname(HostName);
    memcpy(&localaddr, Hostent->h_addr_list[0], Hostent->h_length);
    ipHeader.Version_HLen = (4 << 4 | sizeof(ipHeader) / sizeof(unsigned long));
    ipHeader.TOS = 0;
    ipHeader.Length = htons(sizeof(ipHeader) + sizeof(IcmpHeader) + strlen(IcmpData));
    ipHeader.Ident = htons(1);
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = IPPROTO_ICMP;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = localaddr.S_un.S_addr;
    ipHeader.DestinationAddr = inet_addr(DestIpAddr);
    SendIcmp.Type = 0;
    SendIcmp.Code = 0;
    SendIcmp.ID = htons(1000);
    SendIcmp.Sequence = htons(1);
    SendIcmp.Checksum = 0;
    memcpy(IcmpBuffer, &SendIcmp, sizeof(struct IcmpHeader));
    memcpy(IcmpBuffer + sizeof(struct IcmpHeader), IcmpData, strlen(IcmpData));
    SendIcmp.Checksum = checksum((USHORT*)IcmpBuffer, sizeof(struct IcmpHeader) + strlen(IcmpData));
    memcpy(IcmpBuffer, &ipHeader, sizeof(ipHeader));
    memcpy(IcmpBuffer + sizeof(struct IpHeader), &SendIcmp, sizeof(struct IcmpHeader));
    memcpy(IcmpBuffer + sizeof(struct IpHeader) + sizeof(struct IcmpHeader), IcmpData, strlen(IcmpData));
    ipHeader.Checksum = checksum((USHORT*)IcmpBuffer, sizeof(struct IpHeader));
    memcpy(IcmpBuffer, &ipHeader, sizeof(ipHeader));
    Result = sendto(IcmpSocket, IcmpBuffer, sizeof(struct IpHeader) + sizeof(struct IcmpHeader) + strlen(IcmpData), 0, (SOCKADDR*) &DestAddr, sizeof(DestAddr));
    if (Result == SOCKET_ERROR)
    {
        printf("sendto failed with error %d \n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("Send ICMP Packet OK.\n");
        printf("Type:%d\n", SendIcmp.Type);
        printf("Code:%d\n", SendIcmp.Code);
        printf("Checksum:%d\n", ntohs(SendIcmp.Checksum));
        printf("ID:%d\n", ntohs(SendIcmp.ID));
        printf("Sequence:%d\n", ntohs(SendIcmp.Sequence));
    }
    if (closesocket(IcmpSocket) == SOCKET_ERROR)
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
