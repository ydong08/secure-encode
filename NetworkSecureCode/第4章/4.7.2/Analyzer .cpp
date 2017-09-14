//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "string.h" 
#include "Winsock2.h"
#include <ws2tcpip.h>    
#include "mstcpip.h"
#include "time.h"
#pragma comment(lib,"WS2_32.lib") 
struct IPHeader
{
    unsigned char Version_HLen;
    unsigned char TOS;
    unsigned short Length;
    unsigned short Ident;
    unsigned short Flags_Offset;
    unsigned char TTL;
    unsigned char Protocol;
    unsigned short Checksum;
    unsigned int SourceAddr;
    unsigned int DestinationAddr;
};
struct TcpHeader
{
    unsigned short SrcPort;
    unsigned short DstPort;
    unsigned int SequenceNum;
    unsigned int Acknowledgment;
    unsigned char HdrLen;
    unsigned char Flags;
    unsigned short AdvertisedWindow;
    unsigned short Checksum;
    unsigned short UrgPtr;
};
struct UdpHeader
{
    unsigned short SrcPort;
    unsigned short DstPort;
    unsigned short Length;
    unsigned short Checksum;
};
struct IcmpHeader
{
    BYTE Type;
    BYTE Code;
    USHORT Checksum;
    USHORT ID;
    USHORT Sequence;
};
struct IcmpHeaderEcho
{
    BYTE Type;
    BYTE Code;
    USHORT Checksum;
    USHORT ID;
    USHORT Sequence;
};
int main(int argc, char **argv)
{
    SOCKET SnifferSocket;
    int Result;
    char Packet[65535] = 
    {
        0
    };
    char Name[255];
    HANDLE hCon;
    WSADATA wsaData;
    DWORD dwBufferLen[10];
    DWORD dwBufferInLen = 1;
    DWORD dwBytesReturned = 0;
    struct hostent *pHostent;
    Result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAStartup failed with error %d\n", Result);
        return 0;
    } SnifferSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (Result == SOCKET_ERROR)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    Result = gethostname(Name, 255);
    if (Result == SOCKET_ERROR)
    {
        printf("gethostname failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    pHostent = (struct hostent*)malloc(sizeof(struct hostent));
    pHostent = gethostbyname(Name);
    SOCKADDR_IN sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(6666);
    memcpy(&sock.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
    Result = bind(SnifferSocket, (PSOCKADDR) &sock, sizeof(sock));
    if (Result == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    } Result = WSAIoctl(SnifferSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    if (Result == SOCKET_ERROR)
    {
        printf("WSAIoctl failed with error %d\n", WSAGetLastError());
        closesocket(SnifferSocket);
        return 0;
    }
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hCon, &bInfo);
    while (true)
    {
        memset(Packet, 0, sizeof(Packet));
        Result = recv(SnifferSocket, Packet, sizeof(Packet), 0);
        if (Result == SOCKET_ERROR)
        {
            printf("recv failed with error %d\n", WSAGetLastError());
            closesocket(SnifferSocket);
            return 0;
        }
        SetConsoleTextAttribute(hCon, 15);
        int static packetcount = 0;
        packetcount++;
        time_t temp;
        time(&temp);
        printf("\n");
        printf("Packet:%d Time:%s", packetcount, ctime(&temp));
        SetConsoleTextAttribute(hCon, bInfo.wAttributes);
        SetConsoleTextAttribute(hCon, 14);
        printf("IP Header:\n");
        struct IPHeader *ip = (struct IPHeader*)Packet;
        int IpHeaderLength = (ip->Version_HLen &0x0f) *4;
        printf("Version:%d\n", ip->Version_HLen >> 4);
        printf("Herader Length:%d\n", (ip->Version_HLen &0x0f) *4);
        printf("TOS:%d\n", ip->TOS);
        printf("Length:%d\n", ntohs(ip->Length));
        printf("Flags:%d\n", ntohs(ip->Flags_Offset) >> 13);
        printf("---Reserved bit:%d\n", (ntohs(ip->Flags_Offset) &0x8000) >> 15);
        printf("---Don't fragment:%d\n", (ntohs(ip->Flags_Offset) &0x4000) >> 14);
        printf("---More fragments:%d\n", (ntohs(ip->Flags_Offset) &0x2000) >> 13);
        printf("Fragment offset:%d\n", ntohs(ip->Flags_Offset) &0x1fff);
        printf("Ident:%d\n", ntohs(ip->Ident));
        printf("TTL:%d\n", ip->TTL);
        printf("Protocol:%d\n", ip->Protocol);
        printf("Checksum:%d\n", ntohs(ip->Checksum));
        SOCKADDR_IN addr;
        addr.sin_addr.s_addr = ip->SourceAddr;
        printf("Source IP:%s\n", inet_ntoa(addr.sin_addr));
        addr.sin_addr.s_addr = ip->DestinationAddr;
        printf("Destination IP:%s\n", inet_ntoa(addr.sin_addr));
        SetConsoleTextAttribute(hCon, bInfo.wAttributes);
        int Protocol = ip->Protocol;
        if (Protocol == 6)
        {
            SetConsoleTextAttribute(hCon, 13);
            int IpHeaderLength = (ip->Version_HLen &0x0f) *4;
            printf("TCP Header:\n");
            struct TcpHeader *tcp = (struct TcpHeader*)(Packet + IpHeaderLength);
            printf("Source Port:%d\n", ntohs(tcp->SrcPort));
            printf("Destination Port:%d\n", ntohs(tcp->DstPort));
            printf("SequenceNum:%u\n", ntohl(tcp->SequenceNum));
            printf("Acknowledgment:%u\n", ntohl(tcp->Acknowledgment));
            printf("Header Length:%d\n", (tcp->HdrLen >> 4) *4);
            printf("Flags:\n");
            printf("---FIN:%d\n", (tcp->Flags &0x01) > 0 ? 1 : 0);
            printf("---SYN:%d\n", (tcp->Flags &0x02) > 0 ? 1 : 0);
            printf("---RST:%d\n", (tcp->Flags &0x04) > 0 ? 1 : 0);
            printf("---PSH:%d\n", (tcp->Flags &0x08) > 0 ? 1 : 0);
            printf("---ACK:%d\n", (tcp->Flags &0x10) > 0 ? 1 : 0);
            printf("---URG:%d\n", (tcp->Flags &0x20) > 0 ? 1 : 0);
            printf("AdvertisedWindow:%d\n", ntohs(tcp->AdvertisedWindow));
            printf("Checksum:%d\n", ntohs(tcp->Checksum));
            printf("UrgPtr:%d\n", ntohs(tcp->UrgPtr));
            SetConsoleTextAttribute(hCon, bInfo.wAttributes);
        } if (Protocol == 17)
        {
            SetConsoleTextAttribute(hCon, 12);
            printf("UDP Header:\n");
            struct UdpHeader *udp = (struct UdpHeader*)(Packet + IpHeaderLength);
            printf("Source Port:%d\n", ntohs(udp->SrcPort));
            printf("Destination Port:%d\n", ntohs(udp->DstPort));
            printf("Length:%u\n", ntohs(udp->Length));
            printf("Checksum:%d\n", ntohs(udp->Checksum));
            SetConsoleTextAttribute(hCon, bInfo.wAttributes);
        }
        if (Protocol == 1)
        {
            SetConsoleTextAttribute(hCon, 11);
            printf("ICMP Header:\n");
            struct IcmpHeader *icmp = (struct IcmpHeader*)(Packet + IpHeaderLength);
            printf("Type:%d\n", icmp->Type);
            printf("Code:%d\n", icmp->Code);
            printf("Checksum:%u\n", ntohs(icmp->Checksum));
            int Type = icmp->Type;
            int Code = icmp->Code;
            if (Type == 0 && Code == 0)
            {
                printf("Echo Reply\n");
                struct IcmpHeaderEcho *echo = (struct IcmpHeaderEcho*)(Packet + IpHeaderLength);
                printf("ID:%d\n", ntohs(echo->ID));
                printf("Sequence:%d\n", ntohs(echo->Sequence));
            } if (Type == 8 && Code == 0)
            {
                printf("Echo Request\n");
                struct IcmpHeaderEcho *echo = (struct IcmpHeaderEcho*)(Packet + IpHeaderLength);
                printf("ID:%d\n", ntohs(echo->ID));
                printf("Sequence:%d\n", ntohs(echo->Sequence));
            }
            if (Type == 13)
                printf("Timestamp Request\n");
            if (Type == 14)
                printf("Timestamp Reply\n");
            if (Type == 17)
                printf("Mask Request\n");
            if (Type == 18)
                printf("Mask Reply\n");
            SetConsoleTextAttribute(hCon, bInfo.wAttributes);
        }
        printf("IP Data:\n");
        unsigned char *IpData = (unsigned char*)Packet + IpHeaderLength;
        int IpTotalLength = ntohs(ip->Length);
        int IpDataLength = IpTotalLength - IpHeaderLength;
        char IpDataOut[65535] = 
        {
            0
        };
        int end = 0;
        if (IpDataLength > 0)
        {
            for (int i = 0; i < IpDataLength; i++)
            {
                SetConsoleTextAttribute(hCon, 14);
                printf("%02X", IpData[i]);
                SetConsoleTextAttribute(hCon, bInfo.wAttributes);
                if (isgraph(IpData[i]))
                    IpDataOut[end] = IpData[i];
                else if (IpData[i] == ' ')
                    IpDataOut[end] = IpData[i];
                else
                    IpDataOut[end] = '.';
                end = end + 1;
                if (i % 16 == 15)
                {
                    SetConsoleTextAttribute(hCon, 10);
                    IpDataOut[end] = 0;
                    printf("   %s", IpDataOut);
                    end = 0;
                    printf("\n");
                    SetConsoleTextAttribute(hCon, bInfo.wAttributes);
                }
            }
            if (end > 0)
            {
                for (int k = end * 2; k < 32; k++)
                {
                    printf(" ");
                }
                IpDataOut[end] = 0;
                SetConsoleTextAttribute(hCon, 10);
                printf("   %s", IpDataOut);
                SetConsoleTextAttribute(hCon, bInfo.wAttributes);
                printf("\n");
            }
        }
    }
    if (closesocket(SnifferSocket) == SOCKET_ERROR)
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
