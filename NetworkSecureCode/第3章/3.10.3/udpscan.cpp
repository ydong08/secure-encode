//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#include <ws2tcpip.h>    
#include "mstcpip.h"
#pragma comment(lib,"WS2_32.lib")
int Stop = 0;
int StartPort = 1029;
int EndPort = 1032;
char *DestIpAddr = "192.168.1.3";
typedef struct IpHeader
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
} Ip_Header;
#define URG 0x20 
#define ACK 0x10 
#define PSH 0x08 
#define RST 0x04 
#define SYN 0x02 
#define FIN 0x01 
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
typedef struct UdpHeader
{
    u_short SrcPort;
    u_short DstPort;
    u_short Length;
    u_short Checksum;
} Udp_Header;
typedef struct IcmpHeader
{
    BYTE i_type;
    BYTE i_code;
    USHORT i_checksum;
    USHORT i_id;
    USHORT i_sequence;
} Icmp_Header;
int PacketUDPICMPAnalyzer(int, char*);
int SendUdpPacket(int, char*);
int main(int argc, char **argv)
{
    SOCKET RecSocket;
    int Result;
    char RecvBuf[65535] = 
    {
        0
    };
    LARGE_INTEGER nFreq;
    char Name[255];
    LARGE_INTEGER StartTime;
    LARGE_INTEGER EndTime;
    HANDLE hCon;
    WSADATA wsaData;
    DWORD dwBufferLen[10];
    DWORD dwBufferInLen = 1;
    DWORD dwBytesReturned = 0;
    struct hostent *pHostent;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    } RecSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (Result == SOCKET_ERROR)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        closesocket(RecSocket);
        return 0;
    }
    Result = gethostname(Name, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        closesocket(RecSocket);
        return 0;
    }
    pHostent = (struct hostent*)malloc(sizeof(struct hostent));
    pHostent = gethostbyname(Name);
    SOCKADDR_IN sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5555);
    memcpy(&sock.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    Result = bind(RecSocket, (PSOCKADDR) &sock, sizeof(sock));
    if (Result == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(RecSocket);
        return 0;
    } Result = WSAIoctl(RecSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAIoctl failed with error %d\n", WSAGetLastError());
        closesocket(RecSocket);
        return 0;
    }
    int Timeout = 2000;
    Result = setsockopt(RecSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    if (Result == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d \n", WSAGetLastError());
        return 0;
    }
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    if (QueryPerformanceFrequency(&nFreq))
    {
        QueryPerformanceCounter(&StartTime);
        for (int k = StartPort; k <= EndPort; k++)
        {
            SendUdpPacket(k, DestIpAddr);
            memset(RecvBuf, 0, sizeof(RecvBuf));
            while (1)
            {
                Result = recv(RecSocket, RecvBuf, sizeof(RecvBuf), 0);
                if (Result == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAETIMEDOUT)
                    {
                        printf("recvfrom failed with error %d \n", WSAGetLastError());
                        return 0;
                    }
                    else
                    {
                        printf("Port %d maybe open1.\n", k);
                        break;
                    }
                }
                static int number = 0;
                Result = PacketUDPICMPAnalyzer(k, RecvBuf);
                if (Result == 0)
                {
                    number++;
                    if (number > 3)
                    {
                        printf("Port %d may be open2.\n", k);
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    number = 0;
                    break;
                }
            }
        }
        SetConsoleTextAttribute(hCon, 14);
        QueryPerformanceCounter(&EndTime);
    }
    double fInterval = EndTime.QuadPart - StartTime.QuadPart;
    printf("Total Time:%fms\n", fInterval *1000 / (double)nFreq.QuadPart);
    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
    if (closesocket(RecSocket) == SOCKET_ERROR)
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

int SendUdpPacket(int Port, char *DestIp)
{
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
        unsigned int SourceAddr;
        unsigned int DestinationAddr;
        u_char Zero;
        u_char Protocol;
        unsigned short UdpLength;
    } PsdTcp_Header;
    typedef struct UdpHeader
    {
        u_short SrcPort;
        u_short DstPort;
        u_short Length;
        u_short Checksum;
    } Udp_Header;
    struct in_addr localaddr;
    char HostName[255];
    struct hostent *Hostent;
    WSADATA wsaData;
    SOCKET SendSocket;
    SOCKADDR_IN addr_in;
    Ip_Header ipHeader;
    Udp_Header udpHeader;
    PsdTcp_Header psdudpHeader;
    char szSendBuf[100] = 
    {
        0
    };
    BOOL flag;
    int nTimeOver;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    if ((SendSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
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
    ipHeader.Length = htons(sizeof(ipHeader) + sizeof(udpHeader));
    ipHeader.Ident = 1;
    ipHeader.Flags_Offset = 0;
    ipHeader.TTL = 128;
    ipHeader.Protocol = IPPROTO_UDP;
    ipHeader.Checksum = 0;
    ipHeader.SourceAddr = localaddr.S_un.S_addr;
    ipHeader.DestinationAddr = inet_addr(DestIp);
    udpHeader.DstPort = htons(Port);
    udpHeader.SrcPort = htons(6666);
    udpHeader.Length = htons(sizeof(udpHeader));
    udpHeader.Checksum = 0;
    psdudpHeader.SourceAddr = ipHeader.SourceAddr;
    psdudpHeader.DestinationAddr = ipHeader.DestinationAddr;
    psdudpHeader.Zero = 0;
    psdudpHeader.Protocol = IPPROTO_UDP;
    psdudpHeader.UdpLength = htons(sizeof(UdpHeader));
    memcpy(szSendBuf, &psdudpHeader, sizeof(psdudpHeader));
    memcpy(szSendBuf + sizeof(psdudpHeader), &udpHeader, sizeof(udpHeader));
    udpHeader.Checksum = checksum((USHORT*)szSendBuf, sizeof(psdudpHeader) + sizeof(udpHeader));
    memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
    memcpy(szSendBuf + sizeof(ipHeader), &udpHeader, sizeof(udpHeader));
    ipHeader.Checksum = checksum((USHORT*)szSendBuf, sizeof(ipHeader) + sizeof(udpHeader));
    memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
    Result = sendto(SendSocket, szSendBuf, sizeof(ipHeader) + sizeof(udpHeader), 0, (struct sockaddr*) &addr_in, sizeof(addr_in));
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    } if (closesocket(SendSocket) == SOCKET_ERROR)
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

int PacketUDPICMPAnalyzer(int Port, char *PacketBuffer)
{
    Ip_Header *pIpheader;
    int iProtocol, iTTL;
    char protocolstr[100] = "\0";
    char szSourceIP[16], szDestIP[16];
    SOCKADDR_IN saSource, saDest;
    pIpheader = (Ip_Header*)PacketBuffer;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    iProtocol = pIpheader->Protocol;
    saSource.sin_addr.s_addr = pIpheader->SourceAddr;
    ::strcpy(szSourceIP, inet_ntoa(saSource.sin_addr));
    saDest.sin_addr.s_addr = pIpheader->DestinationAddr;
    ::strcpy(szDestIP, inet_ntoa(saDest.sin_addr));
    iTTL = pIpheader->TTL;
    int iIphLen = sizeof(unsigned long)*(pIpheader->Version_HLen &0x0f);
    if (pIpheader->SourceAddr == inet_addr(DestIpAddr))
    {
        if (iProtocol == IPPROTO_ICMP)
        {
            Icmp_Header *icmp;
            icmp = (Icmp_Header*)(PacketBuffer + sizeof(Ip_Header));
            if ((icmp->i_type == 3) && (icmp->i_code == 3))
            {
                printf("Port %d Close\n", Port);
                return 1;
            }
            else
            {
                printf("Port %d Unknown\n", Port);
                return 1;
            }
        }
        else if (iProtocol == IPPROTO_UDP)
        {
            printf("Port %d Open\n", Port);
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
    return 1;
}
