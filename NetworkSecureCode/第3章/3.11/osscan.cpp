//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#include <ws2tcpip.h>  
#include "mstcpip.h"
#include <time.h>
#pragma comment( lib, "ws2_32.lib");
char DestIpAddr[100] = "192.168.1.3";
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
    BYTE i_type;
    BYTE i_code;
    USHORT i_checksum;
    USHORT i_id;
    USHORT i_sequence;
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

int SendIcmpEchoPacket(void)
{
    WSADATA wsaData;
    sockaddr_in DestAddr;
    Ip_Header *ip;
    Icmp_Header *icmp;
    int Timeout = 1000;
    char IcmpBuffer[8] = "";
    SOCKET IcmpSocket;
    char RecvBuffer[1024];
    sockaddr_in addr;
    int Len = sizeof(addr);
    int Result;
    Icmp_Header *SendIcmp;
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
    SendIcmp->i_type = 8;
    SendIcmp->i_code = 10;
    SendIcmp->i_id = (USHORT)GetCurrentProcessId();
    SendIcmp->i_sequence = 1;
    SendIcmp->i_checksum = 0;
    SendIcmp->i_checksum = checksum((USHORT*)IcmpBuffer, sizeof(IcmpBuffer));
    Result = sendto(IcmpSocket, IcmpBuffer, sizeof(IcmpBuffer), 0, (SOCKADDR*) &DestAddr, sizeof(DestAddr));
    if (Result == SOCKET_ERROR)
    {
        printf("sendto failed with error %d \n", WSAGetLastError());
        return 0;
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

int SendIcmpTimePacket(unsigned char code)
{
    WSADATA wsaData;
    sockaddr_in DestAddr;
    Ip_Header *ip;
    int Timeout = 1000;
    char IcmpBuffer[20] = "";
    SOCKET IcmpSocket;
    char RecvBuffer[1024];
    sockaddr_in addr;
    int Len = sizeof(addr);
    int Result;
    typedef struct IcmpTimeHeader
    {
        BYTE i_type;
        BYTE i_code;
        USHORT i_checksum;
        USHORT i_id;
        USHORT i_sequence;
        unsigned int otime;
        unsigned int rtime;
        unsigned int ttime;
    } Icmp_Time_Header;
    Icmp_Time_Header *SendIcmp;
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
    memset(&DestAddr, 0, sizeof(DestAddr));
    DestAddr.sin_addr.s_addr = inet_addr(DestIpAddr);
    DestAddr.sin_port = htons(0);
    DestAddr.sin_family = AF_INET;
    SendIcmp = (Icmp_Time_Header*)IcmpBuffer;
    SendIcmp->i_type = 13;
    SendIcmp->i_code = code;
    SendIcmp->i_id = (USHORT)GetCurrentProcessId();
    SendIcmp->i_sequence = htons(1);
    SendIcmp->i_checksum = 0;
    SendIcmp->otime = 0;
    SendIcmp->rtime = 0;
    SendIcmp->ttime = 0;
    SendIcmp->i_checksum = checksum((USHORT*)IcmpBuffer, sizeof(IcmpBuffer));
    Result = sendto(IcmpSocket, IcmpBuffer, sizeof(IcmpBuffer), 0, (SOCKADDR*) &DestAddr, sizeof(DestAddr));
    if (Result == SOCKET_ERROR)
    {
        printf("sendto failed with error %d \n", WSAGetLastError());
        return 0;
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

typedef struct IcmpTimeHeader
{
    BYTE i_type;
    BYTE i_code;
    USHORT i_checksum;
    USHORT i_id;
    USHORT i_sequence;
    unsigned int otime;
    unsigned int rtime;
    unsigned int ttime;
} Icmp_Time_Header;
int main(int argc, char *argv[])
{
    WSADATA wsaData;
    sockaddr_in DestAddr;
    Ip_Header *ip;
    Icmp_Header *SendIcmp;
    int Timeout = 1000;
    char IcmpBuffer[8] = "";
    SOCKET IcmpSocket;
    char RecvBuffer[1024];
    sockaddr_in addr;
    int Len = sizeof(addr);
    int Result;
    bool Debug = false;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    IcmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (IcmpSocket == INVALID_SOCKET)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    char Name[255];
    Result = gethostname(Name, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        closesocket(IcmpSocket);
        return 0;
    }
    struct hostent *pHostent;
    pHostent = (struct hostent*)malloc(sizeof(struct hostent));
    pHostent = gethostbyname(Name);
    SOCKADDR_IN sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5555);
    memcpy(&sock.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    Result = bind(IcmpSocket, (PSOCKADDR) &sock, sizeof(sock));
    if (Result == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(IcmpSocket);
        return 0;
    } DWORD dwBufferLen[10];
    DWORD dwBufferInLen = 1;
    DWORD dwBytesReturned = 0;
    Result = WSAIoctl(IcmpSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    Result = setsockopt(IcmpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    if (Result == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d \n", WSAGetLastError());
        return 0;
    }
    SendIcmpEchoPacket();
    bool EchoReply = false;
    unsigned char TTL = 0;
    unsigned char tos = 0;
    while (1)
    {
        memset(RecvBuffer, 0, sizeof(RecvBuffer));
        memset(&addr, 0, sizeof(addr));
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
                static int i;
                if (Debug)
                {
                    printf("recvfrom time out\n", DestIpAddr);
                }
                i++;
                if (i >= 3)
                {
                    EchoReply = false;
                    printf("EchoReply=false\n");
                    break;
                }
            }
        }
        if (Result < sizeof(Ip_Header) + sizeof(Icmp_Header))
        {
            printf("data error from %d\n", inet_ntoa(addr.sin_addr));
        }
        ip = (Ip_Header*)RecvBuffer;
        if ((ip->SourceAddr == inet_addr(DestIpAddr)) && (ip->Protocol == IPPROTO_ICMP))
        {
            Icmp_Header *icmp;
            icmp = (Icmp_Header*)(RecvBuffer + sizeof(Ip_Header));
            if (Debug)
            {
                printf("type:%d\n", icmp->i_type);
                printf("code:%d\n", icmp->i_code);
            }
            if ((icmp->i_type == 0) && (icmp->i_id == GetCurrentProcessId()))
            {
                TTL = ip->TTL;
                if (Debug)
                {
                    printf("TTL=%d\n", TTL);
                }
                tos = ip->TOS;
                if (Debug)
                {
                    printf("TOS=%d\n", tos);
                }
                if (icmp->i_code == 0)
                {
                    EchoReply = true;
                    printf("EchoReply true\n");
                }
                else
                {
                    EchoReply = false;
                    printf("EchoReply false\n");
                }
                if (Debug)
                {
                    printf("Echo Reply.\n", DestIpAddr);
                }
                break;
            }
            else
            {
                continue;
            }
        }
    }
    SendIcmpTimePacket(0);
    bool TimeStampNormal = false;
    while (1)
    {
        memset(RecvBuffer, 0, sizeof(RecvBuffer));
        memset(&addr, 0, sizeof(addr));
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
                static int i;
                if (Debug)
                {
                    printf("recvfrom time out\n", DestIpAddr);
                }
                i++;
                if (i >= 3)
                {
                    TimeStampNormal = false;
                    printf("TimeStampNormal=false\n");
                    break;
                }
            }
        }
        if (Result < sizeof(Ip_Header) + sizeof(Icmp_Header))
        {
            printf("data error from %d\n", inet_ntoa(addr.sin_addr));
        }
        ip = (Ip_Header*)RecvBuffer;
        if ((ip->SourceAddr == inet_addr(DestIpAddr)) && (ip->Protocol == IPPROTO_ICMP))
        {
            Icmp_Time_Header *icmp;
            icmp = (Icmp_Time_Header*)(RecvBuffer + sizeof(Ip_Header));
            if (Debug)
            {
                printf("type:%d\n", icmp->i_type);
                printf("code:%d\n", icmp->i_code);
            }
            if ((icmp->i_type == 14) && (icmp->i_id == GetCurrentProcessId()))
            {
                TimeStampNormal = true;
                printf("TimeStamp true\n");
                if (Debug)
                {
                    printf("%s:timestamp Reply.\n", DestIpAddr);
                    printf("%d ms\n", icmp->ttime);
                }
                break;
            }
            else
            {
                continue;
            }
        }
    }
    SendIcmpTimePacket(2);
    bool TimeStampAbnormal = false;
    while (1)
    {
        memset(RecvBuffer, 0, sizeof(RecvBuffer));
        memset(&addr, 0, sizeof(addr));
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
                static int number = 0;
                if (Debug)
                {
                    printf("recvfrom time out\n", DestIpAddr);
                }
                number++;
                if (number >= 3)
                {
                    TimeStampAbnormal = true;
                    printf("TimeStampAbnormal is true\n");
                    break;
                }
            }
        }
        if (Result < sizeof(Ip_Header) + sizeof(Icmp_Header))
        {
            printf("data error from %d\n", inet_ntoa(addr.sin_addr));
        }
        ip = (Ip_Header*)RecvBuffer;
        if ((ip->SourceAddr == inet_addr(DestIpAddr)) && (ip->Protocol == IPPROTO_ICMP))
        {
            Icmp_Time_Header *icmp;
            icmp = (Icmp_Time_Header*)(RecvBuffer + sizeof(Ip_Header));
            printf("type:%d\n", icmp->i_type);
            printf("code:%d\n", icmp->i_code);
            if ((icmp->i_type == 14) && (icmp->i_id == GetCurrentProcessId()))
            {
                TimeStampAbnormal = false;
                printf("TimeStampAbnormal true\n");
                if (Debug)
                {
                    printf("%s:timestamp Reply.\n", DestIpAddr);
                    printf("%d ms\n", icmp->ttime);
                }
                break;
            }
            else
            {
                continue;
            }
        }
    }
    if (EchoReply)
    {
        if (EchoReply && (TTL <= 32))
        {
            printf("%s OS may be Windows 95\n", DestIpAddr);
        }
        if (EchoReply && TimeStampNormal && TimeStampAbnormal && (TTL == 128))
        {
            printf("%s OS may be Windows 2000\n", DestIpAddr);
        }
        else
        {
            printf("%s OS may be not Windows 2000 \n", DestIpAddr);
        }
    }
    else
    {
        printf("%s OS may be not Windows system\n", DestIpAddr);
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
