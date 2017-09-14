//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <winsock2.h>
#include <stdio.h>
#pragma comment( lib, "ws2_32.lib" )  
void main()
{
    WSADATA wsaData;
    SOCKET ClientSocket;
    SOCKADDR_IN ServerAddr;
    int ServerPort = 4000;
    char *SendBuffer = "Hello World!";
    char ReceiveBuffer[1024] = "";
    int Result;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return ;
    }
    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ClientSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return ;
    }
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(ServerPort);
    ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.9");
    int len;
    if (len = sendto(ClientSocket, SendBuffer, strlen(SendBuffer), 0, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr)) < 0)
    {
        printf("sendto failed with error %d\n", WSAGetLastError());
        return ;
    }
    else
    {
        printf("Send the data:%s\n", SendBuffer);
    }
    int FromAddrSize = sizeof(ServerAddr);
    memset(ReceiveBuffer, 0, sizeof(ReceiveBuffer));
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    if (len = recvfrom(ClientSocket, ReceiveBuffer, 1024, 0, (SOCKADDR*) &ServerAddr, &FromAddrSize) < 0)
    {
        printf("recvfrom failed with error %d\n", WSAGetLastError());
        return ;
    }
    else
    {
        printf("Received Data:%s From :%s\n", ReceiveBuffer, inet_ntoa(ServerAddr.sin_addr));
    }
    if (closesocket(ClientSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
    }
}
