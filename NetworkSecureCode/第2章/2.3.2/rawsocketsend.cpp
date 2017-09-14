//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#include <ws2tcpip.h>  
#pragma comment( lib, "ws2_32.lib");
typedef struct IpHeader
{
    u_char Version_HLen;
    u_char TOS;
    short Length;
    short Ident;
    short Flags_Offset;
    u_char TTL;
    u_char Protocol;
    short Checksum;
    unsigned int SourceAddr;
    unsigned int DestinationAddr;
} Ip_Header;
typedef struct PsdTcpHeader
{
    unsigned long SourceAddr;
    unsigned long DestinationAddr;
    char Zero;
    char Protocol;
    unsigned short TcpLength;
} PSD_Tcp_Header;
typedef struct TcpHeader
{
    USHORT SrcPort;
    USHORT DstPort;
    unsigned int SequenceNum;
    unsigned int Acknowledgment;
    unsigned char HdrLen;
    unsigned char Flags;
    USHORT AdvertisedWindow;
    USHORT Checksum;
    USHORT UrgPtr;
} Tcp_Header;
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
int main()
{
    char *Target = "192.168.1.3";
    struct in_addr localaddr;
    char HostName[255];
    struct hostent *Hostent;
    WSADATA wsaData;
    SOCKET SendSocket;
    SOCKADDR_IN addr_in;
    Ip_Header ipHeader;
    Tcp_Header tcpHeader;
    PSD_Tcp_Header psdHeader;
    char szSendBuf[100] = 
    {
        0
    };
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
    addr_in.sin_addr.S_un.S_addr = inet_addr(Target);
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
    ipHeader.Length = htons(sizeof(ipHeader) + sizeof(tcpHeader));
    ipHeader.Ident = htons(1);
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = IPPROTO_TCP;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = inet_addr("192.168.1.4");
    ipHeader.DestinationAddr = inet_addr(Target);
    tcpHeader.DstPort = htons(80);
    tcpHeader.SrcPort = htons(6666);
    tcpHeader.SequenceNum = 0;
    tcpHeader.Acknowledgment = 0;
    tcpHeader.HdrLen = (sizeof(tcpHeader) / 4 << 4 | 0);
    tcpHeader.Flags = 2;
    tcpHeader.AdvertisedWindow = htons(512);
    tcpHeader.UrgPtr = 0;
    tcpHeader.Checksum = 0;
    psdHeader.SourceAddr = ipHeader.SourceAddr;
    psdHeader.DestinationAddr = ipHeader.DestinationAddr;
    psdHeader.Zero = 0;
    psdHeader.Protocol = IPPROTO_TCP;
    psdHeader.TcpLength = htons(sizeof(tcpHeader));
    memcpy(szSendBuf, &psdHeader, sizeof(psdHeader));
    memcpy(szSendBuf + sizeof(psdHeader), &tcpHeader, sizeof(tcpHeader));
    tcpHeader.Checksum = checksum((USHORT*)szSendBuf, sizeof(psdHeader) + sizeof(tcpHeader));
    memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
    memcpy(szSendBuf + sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader));
    ipHeader.Checksum = checksum((USHORT*)szSendBuf, sizeof(ipHeader));
    memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
    Result = sendto(SendSocket, szSendBuf, sizeof(ipHeader) + sizeof(tcpHeader), 0, (struct sockaddr*) &addr_in, sizeof(addr_in));
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("Send a Tcp data\n");
        printf("IP Header--------:\n");
        printf("Ip Tos:%d\n", ipHeader.TOS);
        printf("IP Length:%d\n", ntohs(ipHeader.Length));
        printf("IP Ident:%d\n", ntohs(ipHeader.Ident));
        printf("IP TTL:%d\n", ipHeader.TTL);
        printf("IP Protocol:%d\n", ipHeader.Protocol);
        printf("IP Checksum:%d\n", ntohs(ipHeader.Checksum));
        struct in_addr a;
        a.s_addr = ipHeader.SourceAddr;
        printf("IP SourceAddr:%s\n", inet_ntoa(a));
        a.s_addr = ipHeader.DestinationAddr;
        printf("IP DestinationAddr:%s\n", inet_ntoa(a));
        printf("TCP Header--------:\n");
        printf("TCP DstPort:%d\n", ntohs(tcpHeader.DstPort));
        printf("TCP SrcPort:%d\n", ntohs(tcpHeader.SrcPort));
        printf("TCP SequenceNum:%d\n", ntohl(tcpHeader.SequenceNum));
        printf("TCP Acknowledgment:%d\n", ntohs(tcpHeader.Acknowledgment));
        unsigned char h;
        h = tcpHeader.HdrLen;
        h = h >> 4;
        printf("Tcp HerderLength:%d\n", 4 *h);
        printf("Tcp Flags:%d\n", tcpHeader.Flags);
        printf("TCP AdvertisedWindow:%d\n", ntohs(tcpHeader.AdvertisedWindow));
        printf("TCP UrgPtr:%d\n", ntohs(tcpHeader.UrgPtr));
        printf("TCP Checksum:%d\n", ntohs(tcpHeader.Checksum));
    }
    if (closesocket(SendSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
        return 0;
    } if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 0;
    }
    return 1;
}
