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
    SOCKET NewSocket;
    SOCKADDR_IN ServerAddr;
    SOCKADDR_IN ClientAddr;
    int ClientAddrLen;
    int ServerPort = 4000;
    char ReceiveBuffer[1024];
    int Result;
    int ReceiveLen;
    if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return ;
    }
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return ;
    }
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(ServerPort);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Result = bind(ServerSocket, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr));
    if (Result < 0)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return ;
    }
    Result = listen(ServerSocket, 5);
    if (Result == SOCKET_ERROR)
    {
        printf("listen failed with error %d\n", WSAGetLastError());
        return ;
    }
    ClientAddrLen = sizeof(SOCKADDR);
    int SendLength;
    do
    {
        NewSocket = accept(ServerSocket, (SOCKADDR*) &ClientAddr, &ClientAddrLen);
        if (NewSocket < 0)
        {
            printf("accept failed with error %d\n", WSAGetLastError());
            return ;
        }
        else
        do
        {
            memset(ReceiveBuffer, 0, sizeof(ReceiveBuffer));
            if ((ReceiveLen = recv(NewSocket, ReceiveBuffer, 1024, 0)) < 0)
            {
                printf("recv failed with error %d\n", WSAGetLastError());
                return ;
            }
            if (ReceiveLen == 0)
                printf("Receive Data Finished\n");
            else
            {
                ReceiveBuffer[1024] = '\0';
                printf("Receive Data:%s\n", ReceiveBuffer);
                SendLength = send(NewSocket, ReceiveBuffer, strlen(ReceiveBuffer), 0);
                if (SendLength < 0)
                {
                    printf("send failed with error %d\n", WSAGetLastError());
                    return ;
                }
                else
                {
                    printf("Send Data:%s\n", ReceiveBuffer);
                }
            }
        }
        while (ReceiveLen != 0);
        if (closesocket(NewSocket) == SOCKET_ERROR)
        {
            printf("closesocket failed with error %d\n", WSAGetLastError());
        };
    }
    while (1)
        ;
    if (closesocket(ServerSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
    }
}
