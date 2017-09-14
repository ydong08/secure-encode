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
struct TcpHeader
{
    unsigned short SrcPort;
    unsigned short DstPort;
    unsigned int SequenceNum;
    unsigned int Acknowledgment;
    unsigned char HdrLen;
    unsigned char Flags;
    unsigned short AdvertisedWindow;
    unsigned short Checksum;
    unsigned short UrgPtr;
};
struct PsdTcpHeader
{
    unsigned long SourceAddr;
    unsigned long DestinationAddr;
    char Zero;
    char Protcol;
    unsigned short TcpLen;
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
int main(int argc, char **argv)
{
    char *DestIpAddr = "192.168.1.3";
    int Port = 90;
    struct in_addr localaddr;
    char HostName[255];
    struct hostent *Hostent;
    WSADATA wsaData;
    SOCKET SendSocket;
    SOCKADDR_IN addr_in;
    struct IpHeader ipHeader;
    struct TcpHeader tcpHeader;
    struct PsdTcpHeader psdHeader;
    char SocketSendBuffer[200] = 
    {
        0
    };
    char TcpData[] = "Here is tcp data content.";
    BOOL flag;
    int nTimeOver;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
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
    addr_in.sin_port = htons(7000);
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
    ipHeader.Length = htons(sizeof(ipHeader) + sizeof(tcpHeader) + strlen(TcpData));
    ipHeader.Ident = htons(1);
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = IPPROTO_TCP;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = localaddr.S_un.S_addr;
    ipHeader.DestinationAddr = inet_addr(DestIpAddr);
    tcpHeader.DstPort = htons(Port);
    tcpHeader.SrcPort = htons(7000);
    tcpHeader.SequenceNum = htonl(0);
    tcpHeader.Acknowledgment = 0;
    tcpHeader.HdrLen = (sizeof(tcpHeader) / 4 << 4 | 0);
    tcpHeader.Flags = 0x18;
    tcpHeader.AdvertisedWindow = htons(512);
    tcpHeader.UrgPtr = 0;
    tcpHeader.Checksum = 0;
    psdHeader.SourceAddr = ipHeader.SourceAddr;
    psdHeader.DestinationAddr = ipHeader.DestinationAddr;
    psdHeader.Zero = 0;
    psdHeader.Protcol = IPPROTO_TCP;
    psdHeader.TcpLen = htons(sizeof(tcpHeader) + strlen(TcpData));
    memcpy(SocketSendBuffer, &psdHeader, sizeof(struct PsdTcpHeader));
    memcpy(SocketSendBuffer + sizeof(struct PsdTcpHeader), &tcpHeader, sizeof(struct TcpHeader));
    memcpy(SocketSendBuffer + sizeof(struct PsdTcpHeader) + sizeof(struct TcpHeader), TcpData, strlen(TcpData));
    tcpHeader.Checksum = checksum((USHORT*)SocketSendBuffer, sizeof(struct PsdTcpHeader) + sizeof(struct TcpHeader) + strlen(TcpData));
    memcpy(SocketSendBuffer, &ipHeader, sizeof(struct IpHeader));
    memcpy(SocketSendBuffer + sizeof(struct IpHeader), &tcpHeader, sizeof(struct TcpHeader));
    memcpy(SocketSendBuffer + sizeof(struct IpHeader) + sizeof(struct TcpHeader), TcpData, strlen(TcpData));
    ipHeader.Checksum = checksum((USHORT*)SocketSendBuffer, sizeof(struct IpHeader));
    memcpy(SocketSendBuffer, &ipHeader, sizeof(ipHeader));
    Result = sendto(SendSocket, SocketSendBuffer, sizeof(struct IpHeader) + sizeof(struct TcpHeader) + strlen(TcpData), 0, (struct sockaddr*) &addr_in, sizeof(addr_in));
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("Send Tcp Packet OK%d.\n", Result);
        printf("Dstination Port:%d\n", ntohs(tcpHeader.DstPort));
        printf("Source Port:%d\n", ntohs(tcpHeader.SrcPort));
        printf("Sequence:%d\n", ntohl(tcpHeader.SequenceNum));
        printf("Acknowledgment:%d\n", ntohl(tcpHeader.Acknowledgment));
        printf("Header Length:%d*4\n", tcpHeader.HdrLen >> 4);
        printf("Flags:0x%0x\n", tcpHeader.Flags);
        printf("AdvertiseWindow:%d\n", ntohs(tcpHeader.AdvertisedWindow));
        printf("UrgPtr:%d\n", ntohs(tcpHeader.UrgPtr));
        printf("Checksum:%u\n", ntohs(tcpHeader.Checksum));
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
