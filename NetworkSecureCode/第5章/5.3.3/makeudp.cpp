//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------// 
#include "stdio.h"
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
struct PsdUdpHeader
{
    unsigned int SourceAddr;
    unsigned int DestinationAddr;
    u_char Zero;
    u_char Protocol;
    unsigned short UdpLength;
};
struct UdpHeader
{
    u_short SrcPort;
    u_short DstPort;
    u_short Length;
    u_short Checksum;
};
USHORT checksum(USHORT *buffer, int size)
{
    unsigned long cksum = 0;
    while (size > 1)
    {
        cksum +=  *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum &0xffff);
    cksum += (cksum >> 16);
    return (USHORT)(~cksum);
}

int main(int argc, char **argv)
{
    int Port = 25;
    char *DestIpAddr = "192.168.1.3";
    struct in_addr HomeIP;
    char HostName[255];
    struct hostent *Hostent;
    WSADATA wsaData;
    SOCKET UDPSocket;
    SOCKADDR_IN addr_in;
    struct IpHeader ipHeader;
    struct UdpHeader udpHeader;
    struct PsdUdpHeader psdudpHeader;
    char SendUDPBuffer[100] = 
    {
        0
    };
    char UdpData[] = "UDP DATA UDP DATA UDP DATA";
    BOOL flag;
    int nTimeOver;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    if ((UDPSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("WSASocket failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    flag = true;
    if (setsockopt(UDPSocket, IPPROTO_IP, IP_HDRINCL, (char*) &flag, sizeof(flag)) == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d\n\n", WSAGetLastError());
        return false;
    }
    nTimeOver = 1000;
    if (setsockopt(UDPSocket, SOL_SOCKET, SO_SNDTIMEO, (char*) &nTimeOver, sizeof(nTimeOver)) == SOCKET_ERROR)
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
    memcpy(&HomeIP, Hostent->h_addr_list[0], Hostent->h_length);
    ipHeader.Version_HLen = (4 << 4 | sizeof(ipHeader) / sizeof(unsigned long));
    ipHeader.TOS = 0;
    ipHeader.Length = htons(sizeof(ipHeader) + sizeof(udpHeader) + strlen(UdpData));
    ipHeader.Ident = 1;
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = IPPROTO_UDP;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = HomeIP.S_un.S_addr;
    ipHeader.DestinationAddr = inet_addr(DestIpAddr);
    udpHeader.DstPort = htons(Port);
    udpHeader.SrcPort = htons(6666);
    udpHeader.Length = htons(sizeof(udpHeader) + strlen(UdpData));
    udpHeader.Checksum = 0;
    psdudpHeader.SourceAddr = ipHeader.SourceAddr;
    psdudpHeader.DestinationAddr = ipHeader.DestinationAddr;
    psdudpHeader.Zero = 0;
    psdudpHeader.Protocol = IPPROTO_UDP;
    psdudpHeader.UdpLength = htons(sizeof(UdpHeader) + strlen(UdpData));
    memcpy(SendUDPBuffer, &psdudpHeader, sizeof(psdudpHeader));
    memcpy(SendUDPBuffer + sizeof(psdudpHeader), &udpHeader, sizeof(udpHeader));
    memcpy(SendUDPBuffer + sizeof(psdudpHeader) + sizeof(struct UdpHeader), UdpData, strlen(UdpData));
    udpHeader.Checksum = checksum((USHORT*)SendUDPBuffer, sizeof(psdudpHeader) + sizeof(udpHeader) + strlen(UdpData));
    memcpy(SendUDPBuffer, &ipHeader, sizeof(ipHeader));
    memcpy(SendUDPBuffer + sizeof(ipHeader), &udpHeader, sizeof(udpHeader));
    memcpy(SendUDPBuffer + sizeof(ipHeader) + sizeof(udpHeader), UdpData, strlen(UdpData));
    ipHeader.Checksum = checksum((USHORT*)SendUDPBuffer, sizeof(ipHeader));
    memcpy(SendUDPBuffer, &ipHeader, sizeof(ipHeader));
    Result = sendto(UDPSocket, SendUDPBuffer, sizeof(ipHeader) + sizeof(udpHeader) + strlen(UdpData), 0, (struct sockaddr*) &addr_in, sizeof(addr_in));
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("Send UDP Pakcet:\n");
        printf("Source Port:%d\n", ntohs(udpHeader.SrcPort));
        printf("Destination Port:%d\n", ntohs(udpHeader.DstPort));
        printf("Length:%d\n", ntohs(udpHeader.Length));
        printf("Checksum:%d\n", ntohs(udpHeader.Checksum));
    }
    if (closesocket(UDPSocket) == SOCKET_ERROR)
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
