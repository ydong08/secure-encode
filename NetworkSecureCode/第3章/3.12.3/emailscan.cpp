//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "Winsock2.h"
#pragma comment(lib,"WS2_32.lib")
int main()
{
    char DestIpAddr[100] = "192.168.1.4";
    WSADATA wsaData;
    int Result;
    SOCKADDR_IN SockDestAddr;
    SOCKET SMTPSocket;
    int SmtpPort = 25;
    char SmtpInformation[1000];
    int SendLength;
    char root[4][100] = 
    {
        "peter", "jenny", "liuwentao", "a"
    };
    Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    }
    SMTPSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (SMTPSocket < 0)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    int Timeout = 1000;
    Result = setsockopt(SMTPSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    if (Result == SOCKET_ERROR)
    {
        printf("setsockopt failed with error %d \n", WSAGetLastError());
        return 0;
    }
    SockDestAddr.sin_family = AF_INET;
    SockDestAddr.sin_port = htons(SmtpPort);
    SockDestAddr.sin_addr.s_addr = inet_addr(DestIpAddr);
    Result = connect(SMTPSocket, (struct sockaddr*) &SockDestAddr, sizeof(SockDestAddr));
    if (Result < 0)
    {
        printf("%s SMTP Server is Down.\n", DestIpAddr);
        printf("connect failed with error %d\n", WSAGetLastError());
        return 0;
    } 
    else
    {
        printf("%s SMTP Server is up.\n", DestIpAddr);
    }
    memset(SmtpInformation, 0, sizeof(SmtpInformation));
    printf("SMTP Server:\n");
    Result = recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recv failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
    {
        printf("%s\n", SmtpInformation);
    }
    SendLength = send(SMTPSocket, "HELP\r\n", sizeof("HELP\r\n"), 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    memset(SmtpInformation, 0, sizeof(SmtpInformation));
    Result = recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recv failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
    {
        printf("%s\n", SmtpInformation);
    }
    memset(SmtpInformation, 0, sizeof(SmtpInformation));
    SendLength = send(SMTPSocket, "HELO\r\n", 6, 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    Result = recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
    SendLength = send(SMTPSocket, "HELO\r\n", 6, 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    memset(SmtpInformation, 0, sizeof(SmtpInformation));
    Result = recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recv failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
    {
        printf("%s\n", SmtpInformation);
    }
    SendLength = send(SMTPSocket, "MAIL FROM:163@sina.com\r\n", 29, 0);
    if (SendLength < 0)
    {
        printf("send failed with error %d\n", WSAGetLastError());
        return 0;
    }
    memset(SmtpInformation, 0, sizeof(SmtpInformation));
    recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
    if (Result == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAETIMEDOUT)
        {
            printf("recv failed with error %d \n", WSAGetLastError());
            return 0;
        }
        else
        {
            printf("recv time out.\n");
        }
    }
    else
    {
        printf("%s\n", SmtpInformation);
    }
    for (int i = 0; i < 4; i++)
    {
        memset(SmtpInformation, 0, sizeof(SmtpInformation));
        char SendRCPT[100];
        sprintf(SendRCPT, "rcpt to:%s\r\n", root[i]);
        SendLength = send(SMTPSocket, SendRCPT, strlen(SendRCPT), 0);
        if (SendLength < 0)
        {
            printf("send failed with error %d\n", WSAGetLastError());
            return 0;
        }
        Result = recv(SMTPSocket, SmtpInformation, sizeof(SmtpInformation), 0);
        if (strstr(SmtpInformation, "250") != NULL)
        {
            printf("Received:%s\n", SmtpInformation);
        }
    }
    if (closesocket(SMTPSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error %d\n", WSAGetLastError());
        return 0;
    }
    if (WSACleanup() == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 0;
    }
    return 1;
}
