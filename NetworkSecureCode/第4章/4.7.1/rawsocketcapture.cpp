//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "string.h"
#include "Winsock2.h"
#include <ws2tcpip.h>    
#include "mstcpip.h"
#include "time.h"
#pragma comment(lib,"WS2_32.lib")  
int main(int argc, char **argv)
{
    SOCKET SnifferSocket;
    int Result;
    char Packet[65535] = 
    {
        0
    };
    char Name[255];
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
    } SnifferSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (Result == SOCKET_ERROR)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    Result = gethostname(Name, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    pHostent = (struct hostent*)malloc(sizeof(struct hostent));
    pHostent = gethostbyname(Name);
    SOCKADDR_IN sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5555);
    memcpy(&sock.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    Result = bind(SnifferSocket, (PSOCKADDR) &sock, sizeof(sock));
    if (Result == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    } Result = WSAIoctl(SnifferSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAIoctl failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    while (true)
    {
        memset(Packet, 0, sizeof(Packet));
        Result = recv(SnifferSocket, Packet, sizeof(Packet), 0);
        if (Result == SOCKET_ERROR)
        {
            printf("recv failed with error %d\n", WSAGetLastError());
            closesocket(SnifferSocket);
            return 0;
        }
        int static packetcount = 0;
        packetcount++;
        time_t temp;
        time(&temp);
        printf("Packet:%d Length:%d Time:%s", packetcount, Result, ctime(&temp));
    }
    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
    if (closesocket(SnifferSocket) == SOCKET_ERROR)
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
