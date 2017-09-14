//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "pcap.h"
#pragma comment(lib,"wpcap.lib") 
#pragma comment(lib,"WS2_32.lib")  
struct EthernetHeader
{
    u_char DestMAC[6];
    u_char SourMAC[6];
    u_short EthType;
};
struct IpHeader
{
    unsigned char Version_HLen;
    unsigned char TOS;
    short Length;
    short Ident;
    short Flags_Offset;
    unsigned char TTL;
    unsigned char Protocol;
    short Checksum;
    unsigned int SourceAddr;
    unsigned int DestinationAddr;
};
struct PsdTcpHeader
{
    unsigned long SourceAddr;
    unsigned long DestinationAddr;
    char Zero;
    char Protcol;
    unsigned short TcpLen;
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
unsigned short checksum(unsigned short *data, int length)
{
    unsigned long temp = 0;
    while (length > 1)
    {
        temp +=  *data++;
        length -= sizeof(unsigned short);
    }
    if (length)
    {
        temp += *(unsigned short*)data;
    }
    temp = (temp >> 16) + (temp &0xffff);
    temp += (temp >> 16);
    return (unsigned short)(~temp);
}

int main(int argc, char **argv)
{
    struct EthernetHeader ethernet;
    struct IpHeader ip;
    struct TcpHeader tcp;
    struct PsdTcpHeader ptcp;
    int Result;
    unsigned char SendBuffer[200];
    char TcpData[] = "Tcp Data Test.";
    pcap_if_t *NetwokDevice;
    pcap_t *WinpcapHandle;
    pcap_if_t *Device;
    char WinpcapError[PCAP_ERRBUF_SIZE];
    int DeviceIndex = 0;
    char DeviceName[100][1000];
    int i;
    Result = pcap_findalldevs(&NetwokDevice, WinpcapError);
    if (Result ==  - 1)
    {
        printf("pcap_findalldevs Error");
        return 0;
    } for (Device = NetwokDevice, i = 0; Device && i < 10; Device = Device->next, i++)
    {
        printf("Number %d:", i);
        printf("%s:", Device->name);
        printf("%s\n", Device->description);
        sprintf(DeviceName[i], "%s", Device->name);
    }
    for (;;)
    {
        printf("Please Choose the Device Number:(0-%d)", i - 1);
        scanf("%d", &DeviceIndex);
        if (DeviceIndex > i - 1 || DeviceIndex < 0)
        {
            printf("Device Number Error\n");
            continue;
        }
        else
            break;
    }
    WinpcapHandle = pcap_open_live(LPCTSTR(DeviceName[DeviceIndex]), 65535, 1, 1000, WinpcapError);
    if (WinpcapHandle == NULL)
    {
        printf("pcap_open_live error");
        pcap_freealldevs(NetwokDevice);
        return 0;
    }
    memset(&ethernet, 0, sizeof(ethernet));
    BYTE destmac[8];
    destmac[0] = 0x00;
    destmac[1] = 0x11;
    destmac[2] = 0x22;
    destmac[3] = 0x33;
    destmac[4] = 0x44;
    destmac[5] = 0x55;
    memcpy(ethernet.DestMAC, destmac, 6);
    BYTE hostmac[8];
    hostmac[0] = 0x00;
    hostmac[1] = 0x1a;
    hostmac[2] = 0x4d;
    hostmac[3] = 0x70;
    hostmac[4] = 0xa3;
    hostmac[5] = 0x89;
    memcpy(ethernet.SourMAC, hostmac, 6);
    ethernet.EthType = htons(0x0800);
    memcpy(&SendBuffer, &ethernet, sizeof(struct EthernetHeader));
    ip.Version_HLen = 0x45;
    ip.TOS = 0;
    ip.Length = htons(sizeof(struct IpHeader) + sizeof(struct TcpHeader) + strlen(TcpData));
    ip.Ident = htons(1);
    ip.Flags_Offset = 0;
    ip.TTL = 128;
    ip.Protocol = 6;
    ip.Checksum = 0;
    ip.SourceAddr = inet_addr("192.168.1.9");
    ip.DestinationAddr = inet_addr("192.168.1.24");
    memcpy(&SendBuffer[sizeof(struct EthernetHeader)], &ip, 20);
    tcp.DstPort = htons(88);
    tcp.SrcPort = htons(1000);
    tcp.SequenceNum = htonl(11);
    tcp.Acknowledgment = 0;
    tcp.HdrLen = 0x50;
    tcp.Flags = 0x18;
    tcp.AdvertisedWindow = htons(512);
    tcp.UrgPtr = 0;
    tcp.Checksum = 0;
    memcpy(&SendBuffer[sizeof(struct EthernetHeader) + 20], &tcp, 20);
    ptcp.SourceAddr = ip.SourceAddr;
    ptcp.DestinationAddr = ip.DestinationAddr;
    ptcp.Zero = 0;
    ptcp.Protcol = 6;
    ptcp.TcpLen = htons(sizeof(struct TcpHeader) + strlen(TcpData));
    char TempBuffer[65535];
    memcpy(TempBuffer, &ptcp, sizeof(struct PsdTcpHeader));
    memcpy(TempBuffer + sizeof(struct PsdTcpHeader), &tcp, sizeof(struct TcpHeader));
    memcpy(TempBuffer + sizeof(struct PsdTcpHeader) + sizeof(struct TcpHeader), TcpData, strlen(TcpData));
    tcp.Checksum = checksum((USHORT*)(TempBuffer), sizeof(struct PsdTcpHeader) + sizeof(struct TcpHeader) + strlen(TcpData));
    memcpy(SendBuffer + sizeof(struct EthernetHeader) + sizeof(struct IpHeader), &tcp, sizeof(struct TcpHeader));
    memcpy(SendBuffer + sizeof(struct EthernetHeader) + sizeof(struct IpHeader) + sizeof(struct TcpHeader), TcpData, strlen(TcpData));
    memset(TempBuffer, 0, sizeof(TempBuffer));
    memcpy(TempBuffer, &ip, sizeof(struct IpHeader));
    ip.Checksum = checksum((USHORT*)(TempBuffer), sizeof(struct IpHeader));
    memcpy(SendBuffer + sizeof(struct EthernetHeader), &ip, sizeof(struct IpHeader));
    Result = pcap_sendpacket(WinpcapHandle, SendBuffer, sizeof(struct EthernetHeader) + sizeof(struct IpHeader) + sizeof(struct TcpHeader) + strlen(TcpData));
    if (Result != 0)
    {
        printf("Send Error!\n");
    } 
    else
    {
        printf("Send TCP Packet.\n");
        printf("Dstination Port:%d\n", ntohs(tcp.DstPort));
        printf("Source Port:%d\n", ntohs(tcp.SrcPort));
        printf("Sequence:%d\n", ntohl(tcp.SequenceNum));
        printf("Acknowledgment:%d\n", ntohl(tcp.Acknowledgment));
        printf("Header Length:%d*4\n", tcp.HdrLen >> 4);
        printf("Flags:0x%0x\n", tcp.Flags);
        printf("AdvertiseWindow:%d\n", ntohs(tcp.AdvertisedWindow));
        printf("UrgPtr:%d\n", ntohs(tcp.UrgPtr));
        printf("Checksum:%u\n", ntohs(tcp.Checksum));
    }
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
