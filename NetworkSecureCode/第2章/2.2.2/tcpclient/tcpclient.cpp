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
    int ServerPort = 5150;
    char ServerIPAddr[50] = "192.168.1.9";
    char *SendData = "Hello World!";
    char ReceiveBuffer[1024] = "";
    int SendLength =  - 1;
    int Result =  - 1;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return ;
    }
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return ;
    }
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(ServerPort);
    ServerAddr.sin_addr.s_addr = inet_addr(ServerIPAddr);
    Result = connect(ClientSocket, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr));
    if (Result < 0)
    {
        printf("connect failed with error %d\n", WSAGetLastError());
        return ;
    }
    printf("strlen:%d\n", strlen(SendData));
    SendLength = send(ClientSocket, SendData, strlen(SendData), 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return ;
    }
    else
    {
        printf("Send Data:%s\n", SendData);
    }
    int ReceiveLen;
    memset(ReceiveBuffer, 0, sizeof(ReceiveBuffer));
    if ((ReceiveLen = recv(ClientSocket, ReceiveBuffer, 1024, 0)) < 0)
    {
        printf("recv failed with error %d\n", WSAGetLastError());
        return ;
    }
    else
    {
        printf("Receive Data:%s\n", ReceiveBuffer);
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
