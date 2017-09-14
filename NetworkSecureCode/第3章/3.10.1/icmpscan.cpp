//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#pragma comment( lib, "ws2_32.lib" ) ;
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
typedef struct IcmpHeader
{
    BYTE Type;
    BYTE Code;
    USHORT Checksum;
    USHORT ID;
    USHORT Sequence;
} Icmp_Header;
USHORT checksum(USHORT *buff, int size)
{
    unsigned long cksum = 0;
    while (size > 1)
    {
        cksum +=  *buff++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR*)(buff);
    }
    cksum = (cksum >> 16) + (cksum &0xffff);
    cksum += (cksum >> 16);
    return (USHORT)(~cksum);
}

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    sockaddr_in DestAddr;
    Ip_Header *ip;
    Icmp_Header *icmp;
    Icmp_Header *SendIcmp;
    int Timeout = 1000;
    char DestIpAddr[100] = "192.168.1.3";
    char IcmpBuffer[8] = "";
    SOCKET IcmpSocket;
    char RecvBuffer[1024];
    sockaddr_in addr;
    int Len = sizeof(addr);
    int Result;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    IcmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (IcmpSocket == INVALID_SOCKET)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    Result = setsockopt(IcmpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    if (Result == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d \n", WSAGetLastError());
        return 0;
    }
    memset(&DestAddr, 0, sizeof(DestAddr));
    DestAddr.sin_addr.s_addr = inet_addr(DestIpAddr);
    DestAddr.sin_port = htons(0);
    DestAddr.sin_family = AF_INET;
    SendIcmp = (Icmp_Header*)IcmpBuffer;
    SendIcmp->Type = 8;
    SendIcmp->Code = 0;
    SendIcmp->ID = (USHORT)GetCurrentProcessId();
    SendIcmp->Sequence = htons(1);
    SendIcmp->Checksum = 0;
    SendIcmp->Checksum = checksum((USHORT*)IcmpBuffer, sizeof(IcmpBuffer));
    Result = sendto(IcmpSocket, IcmpBuffer, sizeof(IcmpBuffer), 0, (SOCKADDR*) &DestAddr, sizeof(DestAddr));
    if (Result == SOCKET_ERROR)
    {
        printf("sendto failed with error %d \n", WSAGetLastError());
        return 0;
    }
    Result = recvfrom(IcmpSocket, RecvBuffer, 1024, 0, (sockaddr*) &addr, &Len);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recvfrom failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("Host %s may be down.\n", DestIpAddr);
        }
    }
    if (Result < sizeof(Ip_Header) + sizeof(Icmp_Header))
    {
        printf("data error from %d\n", inet_ntoa(addr.sin_addr));
    }
    ip = (Ip_Header*)RecvBuffer;
    if ((ip->SourceAddr == inet_addr(DestIpAddr)) && (ip->Protocol == IPPROTO_ICMP))
    {
        icmp = (Icmp_Header*)(RecvBuffer + sizeof(Ip_Header));
        if (icmp->Type != 0)
        {
            printf("type error %d ", icmp->Type);
            return 0;
        }
        if (icmp->ID != GetCurrentProcessId())
        {
            printf("id error %d\n", icmp->ID);
            return 0;
        }
        else if ((icmp->Type == 0) && (icmp->ID == GetCurrentProcessId()))
        {
            printf("Host %s is up.\n", DestIpAddr);
        }
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
