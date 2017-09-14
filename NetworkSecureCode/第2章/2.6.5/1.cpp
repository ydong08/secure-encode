//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "winsock.h" 
#include <stdio.h>
#pragma comment(lib,"Ws2_32.lib")
void main()
{
    WSADATA wsaData;
    char HostName[255];
    HOSTENT *Hostent;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return ;
    }
    Result = gethostname(HostName, 255);
    printf("主机名称为：%s\n", HostName);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        return ;
    }
    Hostent = (struct hostent*)malloc(sizeof(struct hostent));
    Hostent = gethostbyname(HostName);
    for (int i = 0;; i++)
    {
        printf("第%d个网络接口:\n", i + 1);
        printf("IP地址：%s\n", inet_ntoa(*(IN_ADDR*)Hostent->h_addr_list[i]));
        if (Hostent->h_addr_list[i] + Hostent->h_length >= Hostent->h_name)
        {
            break;
        } 
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return ;
    }
}
