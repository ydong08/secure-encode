//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
struct Scan
{
    int Port;
};
char *TargetIPAddr = "192.168.1.3";
DWORD WINAPI PortScanTcpConnect(LPVOID lpParam)
{
    SOCKET ConnectSocket;
    struct Scan *p = (struct Scan*)lpParam;
    int Port = p->Port;
    SOCKADDR_IN TargetAddr_in;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    int Result;
    WSADATA wsaData;
    if ((Result = WSAStartup(MAKEWORD(2, 1), &wsaData)) != 0)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    } ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error:%d\n", WSAGetLastError());
        return 0;
    }
    memset(&TargetAddr_in, 0, sizeof(SOCKADDR_IN));
    TargetAddr_in.sin_family = AF_INET;
    TargetAddr_in.sin_addr.s_addr = inet_addr(TargetIPAddr);
    TargetAddr_in.sin_port = htons(Port);
    Result = connect(ConnectSocket, (struct sockaddr*) &TargetAddr_in, sizeof(TargetAddr_in));
    if (Result == SOCKET_ERROR)
    {
        SetConsoleTextAttribute(hCon, 10);
        printf("Port% 5d Close\n", Port);
    } 
    else
    {
        SetConsoleTextAttribute(hCon, 14);
        printf("Port% 5d Open\n", Port);
    }
    if (closesocket(ConnectSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
    return 0;
}
int main(int argc, char **argv)
{
    unsigned int StartPort = 80;
    unsigned int EndPort = 95;
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
    struct Scan p[10];
    HANDLE ThreadHandle[10];
    int number = (EndPort - StartPort + 1) / 10;
    printf("number %d\n", number);
    for (int j = 0; j <= number; j++)
    {
        int CurrentEnd = 0;
        if (j == number)
        {
            CurrentEnd = EndPort;
        } 
        else
        {
            CurrentEnd = (j + 1) *10+StartPort - 1;
        }
        int CurrentStart = j * 10+StartPort;
        for (int k = 0; k < 10; k++)
            ThreadHandle[k] = NULL;
        for (int i = 0; i < CurrentEnd - CurrentStart + 1; i++)
        {
            p[i].Port = i + CurrentStart;
            ThreadHandle[i] = CreateThread(NULL, 0, PortScanTcpConnect, (void*) &p[i], 0, NULL);
            if (ThreadHandle[i] == NULL)
            {
                printf("erorr\n");
            }
        }
        if (WaitForMultipleObjects(CurrentEnd - CurrentStart + 1, ThreadHandle, TRUE, 10000) == WAIT_TIMEOUT)
        {
            printf("TIMEOUT\n");
            for (int i = 0; i < CurrentEnd - CurrentStart + 1; i++)
                TerminateThread(ThreadHandle[i], 1);
        }
        else
        {
            printf("Sucess!\n");
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
