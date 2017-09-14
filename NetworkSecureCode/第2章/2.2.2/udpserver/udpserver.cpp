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
    SOCKET ServerSocket;
    SOCKADDR_IN ServerAddr;
    SOCKADDR_IN ClientAddr;
    int ServerPort = 4000;
    char ReceiveBuffer[1024];
    int ClientAddrSize = sizeof(ClientAddr);
    int Result;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return ;
    }
    ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ServerSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return ;
    }
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(ServerPort);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Result = bind(ServerSocket, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr));
    if (Result < 0)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return ;
    }
    int len;
    while (1)
    {
        memset(ReceiveBuffer, 0, sizeof(ReceiveBuffer));
        memset(&ClientAddr, 0, sizeof(ClientAddr));
        if (len = recvfrom(ServerSocket, ReceiveBuffer, 1024, 0, (SOCKADDR*) &ClientAddr, &ClientAddrSize) < 0)
        {
            printf("recvfrom failed with error %d\n", WSAGetLastError());
            return ;
        }
        else
        {
            printf("Received Data:%s From :%s\n", ReceiveBuffer, inet_ntoa(ClientAddr.sin_addr));
            len = sendto(ServerSocket, ReceiveBuffer, strlen(ReceiveBuffer), 0, (SOCKADDR*) &ClientAddr, sizeof(ClientAddr));
            if (len < 0)
            {
                printf("sendto failed with error %d\n", WSAGetLastError());
                return ;
            }
            else
            {
                printf("Send the data:%s\n", ReceiveBuffer);
            }
        }
    }
    if (closesocket(ServerSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
    }
}
