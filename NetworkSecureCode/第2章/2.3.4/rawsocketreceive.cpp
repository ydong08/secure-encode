//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#include "mstcpip.h"
#pragma comment( lib, "ws2_32.lib");
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
int main(int argc, char *argv[])
{
    WSADATA wsaData;
    Ip_Header *ip;
    int Timeout = 1000;
    SOCKET ReceiveSocket;
    char RecvBuffer[1024];
    sockaddr_in addr;
    int Len = sizeof(addr);
    int Result;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    ReceiveSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (ReceiveSocket == INVALID_SOCKET)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    char Name[255];
    Result = gethostname(Name, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return 0;
    }
    struct hostent *pHostent;
    pHostent = (struct hostent*)malloc(sizeof(struct hostent));
    pHostent = gethostbyname(Name);
    SOCKADDR_IN sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5555);
    memcpy(&sock.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    Result = bind(ReceiveSocket, (PSOCKADDR) &sock, sizeof(sock));
    if (Result == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(ReceiveSocket);
        return 0;
    } DWORD dwBufferLen[10];
    DWORD dwBufferInLen = 1;
    DWORD dwBytesReturned = 0;
    Result = WSAIoctl(ReceiveSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAIoctl failed with error %d \n", WSAGetLastError());
        return 0;
    }
    while (1)
    {
        memset(RecvBuffer, 0, sizeof(RecvBuffer));
        memset(&addr, 0, sizeof(addr));
        Result = recvfrom(ReceiveSocket, RecvBuffer, 1024, 0, (sockaddr*) &addr, &Len);
        if (Result == SOCKET_ERROR)
        {
            printf("recvfrom failed with error %d \n", WSAGetLastError());
            return 0;
        }
        static int number = 0;
        number++;
        printf("Packet Number: %d\n", number);
        ip = (Ip_Header*)RecvBuffer;
        struct in_addr a;
        a.s_addr = ip->SourceAddr;
        printf("ip source IP: %s\n", inet_ntoa(a));
        a.s_addr = ip->DestinationAddr;
        printf("ip destination IP: %s\n", inet_ntoa(a));
        printf("protocol: %d\n", ip->Protocol);
    }
    if (closesocket(ReceiveSocket) == SOCKET_ERROR)
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
