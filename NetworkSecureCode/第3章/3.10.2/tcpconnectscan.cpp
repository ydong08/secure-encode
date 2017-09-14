//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
int main(int argc, char **argv)
{
    char *TargetIPAddr = "192.168.1.3";
    unsigned int StartPort = 80;
    unsigned int EndPort = 90;
    SOCKET ScanSocket;
    struct sockaddr_in TargetAddr_in;
    int Ret;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    WSADATA wsaData;
    if ((Ret = WSAStartup(MAKEWORD(2, 1), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Ret);
        return 0;
    } DWORD dwStart = GetTickCount();
    printf("Target IP:%s\n", TargetIPAddr);
    for (unsigned int i = StartPort; i <= EndPort; i++)
    {
        ScanSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ScanSocket == INVALID_SOCKET)
        {
            printf("socket failed with error:%d\n", WSAGetLastError());
            return 0;
        }
        TargetAddr_in.sin_family = AF_INET;
        TargetAddr_in.sin_addr.s_addr = inet_addr(TargetIPAddr);
        TargetAddr_in.sin_port = htons(i);
        if (connect(ScanSocket, (struct sockaddr*) &TargetAddr_in, sizeof(TargetAddr_in)) == SOCKET_ERROR)
        {
            SetConsoleTextAttribute(hCon, 10);
            printf("Port% 5d Close\n", i);
        } 
        else
        {
            SetConsoleTextAttribute(hCon, 14);
            printf("Port% 5d Open\n", i);
        }
        if (closesocket(ScanSocket) == SOCKET_ERROR)
        {
            printf("closesocket failed with error %d\n", WSAGetLastError());
            return 0;
        }
    }
    SetConsoleTextAttribute(hCon, 14);
    printf("\ntime: %dms \n", GetTickCount() - dwStart);
    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 0;
    }
    return 1;
}
