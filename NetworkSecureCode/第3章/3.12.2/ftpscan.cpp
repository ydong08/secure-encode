//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#pragma comment(lib,"WS2_32.lib")
struct userpassword
{
    char *user;
    char *password;
};
int main()
{
    char *DestIpAddr = "192.168.1.3";
    int FtpPort = 21;
    char FtpResponse[1000];
    SOCKET FtpSocket;
    SOCKADDR_IN addr;
    char *SendStr = NULL;
    struct userpassword UserPassword[4] = 
    {
        {
            "peter", "123"
        } , 
        {
            "computer", "1234"
        }
        , 
        {
            "www", "3333"
        }
        , 
        {
            "me", "888"
        }
    };
    int Result;
    int SendLength;
    WSADATA wsaData;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    FtpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (FtpSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    int Timeout = 1000;
    Result = setsockopt(FtpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    if (Result == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d \n", WSAGetLastError());
        return 0;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(FtpPort);
    addr.sin_addr.s_addr = inet_addr(DestIpAddr);
    Result = connect(FtpSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0;
    if (Result < 0)
    {
        printf("connect failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("%s Ftp Server is up.\n", DestIpAddr);
    }
    memset(FtpResponse, 0, sizeof(FtpResponse));
    Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recvfrom failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
        printf("%s\n", FtpResponse);
    memset(FtpResponse, 0, sizeof(FtpResponse));
    SendStr = "USER anonymous\r\n";
    SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recvfrom failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
    {
        printf("%s\n", FtpResponse);
        if (strstr(FtpResponse, "331"))
        {
            printf("Find User anonymous.\n");
            SendStr = "PASS test@163.com\r\n";
            SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
            if (SendLength < 0)
            {
                printf("send failed with error %d\n", WSAGetLastError());
                return 0;
            }
            memset(FtpResponse, 0, sizeof(FtpResponse));
            Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
            if (Result == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAETIMEDOUT)
                {
                    printf("recvfrom failed with error %d \n", WSAGetLastError());
                    return 0;
                }
                else
                {
                    printf("recv time out.\n");
                }
            }
            printf("%s\n", FtpResponse);
        }
        else
        {
            printf("Not Find User anonymous.\n");
        }
    }
    SendStr = "QUIT\r\n";
    SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    memset(FtpResponse, 0, sizeof(FtpResponse));
    Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recvfrom failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    printf("%s\n", FtpResponse);
    if (closesocket(FtpSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        FtpSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (FtpSocket < 0)
        {
            printf("socket failed with error %d\n", WSAGetLastError());
            return 0;
        }
        int Timeout = 1000;
        Result = setsockopt(FtpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
        if (Result == SOCKET_ERROR)
        {
            printf("setsockopt failed with error %d \n", WSAGetLastError());
            return 0;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(FtpPort);
        addr.sin_addr.s_addr = inet_addr(DestIpAddr);
        Result = connect(FtpSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0;
        if (Result < 0)
        {
            printf("connect failed with error %d\n", WSAGetLastError());
            return 0;
        } 
        else
        {
            printf("%s Ftp Server is up.\n", DestIpAddr);
        }
        memset(FtpResponse, 0, sizeof(FtpResponse));
        Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
        if (Result == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAETIMEDOUT)
            {
                printf("recvfrom failed with error %d \n", WSAGetLastError());
                return 0;
            }
            else
            {
                printf("recv time out.\n");
            }
        }
        printf("Receive:%s\n", FtpResponse);
        memset(FtpResponse, 0, sizeof(FtpResponse));
        char SendStr[100];
        sprintf(SendStr, "USER %s\r\n", UserPassword[i].user);
        SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
        if (SendLength < 0)
        {
            printf("send failed with error %d\n", WSAGetLastError());
            return 0;
        }
        Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
        if (Result == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAETIMEDOUT)
            {
                printf("recvfrom failed with error %d \n", WSAGetLastError());
                return 0;
            }
            else
            {
                printf("recv time out.\n");
            }
        }
        else
        {
            printf("%s\n", FtpResponse);
            if (strstr(FtpResponse, "331"))
            {
                memset(FtpResponse, 0, sizeof(FtpResponse));
                sprintf(SendStr, "PASS %s\r\n", UserPassword[i].password);
                SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
                if (SendLength < 0)
                {
                    printf("send failed with error %d\n", WSAGetLastError());
                    return 0;
                }
                Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
                if (Result == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAETIMEDOUT)
                    {
                        printf("recvfrom failed with error %d \n", WSAGetLastError());
                        return 0;
                    }
                    else
                    {
                        printf("recv time out.\n");
                    }
                }
                else
                {
                    if (strstr(FtpResponse, "230"))
                    {
                        printf("Find user %s password %s \n", UserPassword[i].user, UserPassword[i].password);
                    }
                    else
                    {
                        printf("Not Find User %s  Password %s\n", UserPassword[i].user, UserPassword[i].password);
                    }
                }
            }
        }
        sprintf(SendStr, "%s", "QUIT\r\n");
        SendLength = send(FtpSocket, SendStr, strlen(SendStr), 0);
        if (SendLength < 0)
        {
            printf("send failed with error %d\n", WSAGetLastError());
            return 0;
        }
        memset(FtpResponse, 0, sizeof(FtpResponse));
        Result = recv(FtpSocket, FtpResponse, sizeof(FtpResponse), 0);
        if (Result == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAETIMEDOUT)
            {
                printf("recvfrom failed with error %d \n", WSAGetLastError());
                return 0;
            }
            else
            {
                printf("recv time out.\n");
            }
        }
        printf("%s\n", FtpResponse);
        if (closesocket(FtpSocket) == SOCKET_ERROR)
        {
            printf("closesocket failed with error %d\n", WSAGetLastError());
            return 0;
        }
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 0;
    }
    return 1;
}
