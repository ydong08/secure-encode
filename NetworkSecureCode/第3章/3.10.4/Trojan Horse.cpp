//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
void IpToNum(char *straddr, unsigned char byteaddr[4])
{
    char temp[16];
    char *k;
    memcpy(temp, straddr, strlen(straddr));
    temp[strlen(straddr)] = '\0';
    char *m = temp;
    for (int i = 0; i < 3; i++)
    {
        k = strchr(m, '.');
        char addr[4];
        for (int j = 0; j < (strlen(m) - strlen(k)); j++)
            addr[j] = m[j];
        addr[j] = '\0';
        byteaddr[i] = atoi(addr);
        k = k + 1;
        m = k;
    }
    byteaddr[3] = atoi(m);
}
int main(int argc, char **argv)
{
    char *TargetIpAddrStart = "192.168.1.2";
    char *TargetIpAddrEnd = "192.168.1.4";
    unsigned int StartPort = 7626;
    unsigned int EndPort = 7626;
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
    BYTE ip[4];
    IpToNum(TargetIpAddrStart, ip);
    int IpAddrStart = ip[3];
    IpToNum(TargetIpAddrEnd, ip);
    int IpAddrEnd = ip[3];
    for (int addr = IpAddrStart; addr <= IpAddrEnd; addr++)
    {
        for (unsigned int i = StartPort; i <= EndPort; i++)
        {
            ScanSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (ScanSocket == INVALID_SOCKET)
            {
                printf("socket failed with error:%d\n", WSAGetLastError());
                return 0;
            }
            TargetAddr_in.sin_family = AF_INET;
            ip[3] = addr;
            char CurrentIp[100] = "\0";
            sprintf(CurrentIp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
            TargetAddr_in.sin_addr.s_addr = inet_addr(CurrentIp);
            TargetAddr_in.sin_port = htons(i);
            if (connect(ScanSocket, (struct sockaddr*) &TargetAddr_in, sizeof(TargetAddr_in)) == SOCKET_ERROR)
            {
                SetConsoleTextAttribute(hCon, 10);
                printf("%s Binghe no found.\n", CurrentIp);
            } 
            else
            {
                SetConsoleTextAttribute(hCon, 14);
                printf("%s Binghe may be up!\n", CurrentIp);
            }
            if (closesocket(ScanSocket) == SOCKET_ERROR)
            {
                printf("closesocket failed with error %d\n", WSAGetLastError());
                return 0;
            }
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
