//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdio.h>
#pragma comment(lib,"Ws2_32.lib")
int main()
{
    WSADATA wsaData;
    SOCKET socket;
    INTERFACE_INFO netinterface[10];
    DWORD dwBytesReturned;
    int InterfaceCount;
    int Result;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 1;
    }
    socket = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if (socket == INVALID_SOCKET)
    {
        printf("WSASocket failed with error %d\n\n", WSAGetLastError());
        return 1;
    }
    Result = WSAIoctl(socket, SIO_GET_INTERFACE_LIST, 0, 0, &netinterface, sizeof(netinterface), &dwBytesReturned, 0, 0);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAIoctl failed with error %d \n", WSAGetLastError());
        return 1;
    }
    InterfaceCount = dwBytesReturned / sizeof(INTERFACE_INFO);
    printf("网络接口个数：%d\n", InterfaceCount);
    for (int i = 0; i < InterfaceCount; i++)
    {
        printf("\n第%d个网络接口:\n", i + 1);
        printf("IP地址为：%s\n", inet_ntoa(netinterface[i].iiAddress.AddressIn .sin_addr));
        printf("广播地址为：%s\n", inet_ntoa(netinterface[i].iiBroadcastAddress .AddressIn .sin_addr));
        printf("子网掩码为：%s\n", inet_ntoa(netinterface[i].iiNetmask .AddressIn .sin_addr));
        if (netinterface[i].iiFlags &IFF_POINTTOPOINT)
            printf("Point to Point网络接口\n");
        if (netinterface[i].iiFlags &IFF_LOOPBACK)
            printf("回环接口\n");
        if (netinterface[i].iiFlags &IFF_BROADCAST)
            printf("支持广播\n");
        if (netinterface[i].iiFlags &IFF_MULTICAST)
            printf("支持多播\n");
        if (netinterface[i].iiFlags &IFF_UP)
            printf("接口状态：UP\n");
        else
            printf("接口状态：DOWN\n");
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}
