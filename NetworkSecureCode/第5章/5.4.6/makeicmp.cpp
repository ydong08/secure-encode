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
struct IcmpHeader
{
    unsigned char Type;
    unsigned char Code;
    unsigned short Checksum;
    unsigned short ID;
    unsigned short Sequence;
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
    struct IcmpHeader icmp;
    int Result;
    unsigned char SendBuffer[200];
    char CheckBuffer[65535];
    char IcmpData[] = "Icmp Icmp Icmp Icmp content";
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
    destmac[0] = 0x44;
    destmac[1] = 0x67;
    destmac[2] = 0xa3;
    destmac[3] = 0x4c;
    destmac[4] = 0x53;
    destmac[5] = 0x92;
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
    ip.Length = htons(sizeof(struct IpHeader) + sizeof(struct IcmpHeader) + strlen(IcmpData));
    ip.Ident = htons(1);
    ip.Flags_Offset = 0;
    ip.TTL = 128;
    ip.Protocol = 1;
    ip.Checksum = 0;
    ip.SourceAddr = inet_addr("192.168.1.9");
    ip.DestinationAddr = inet_addr("192.168.1.78");
    memcpy(&SendBuffer[sizeof(struct EthernetHeader)], &ip, 20);
    icmp.Type = 0;
    icmp.Code = 0;
    icmp.ID = htons(12);
    icmp.Sequence = htons(1);
    icmp.Checksum = 0;
    memset(CheckBuffer, 0, sizeof(CheckBuffer));
    memcpy(CheckBuffer, &icmp, sizeof(struct IcmpHeader));
    memcpy(CheckBuffer + sizeof(struct IcmpHeader), IcmpData, strlen(IcmpData));
    icmp.Checksum = checksum((USHORT*)(CheckBuffer), sizeof(struct IcmpHeader) + strlen(IcmpData));
    memcpy(SendBuffer + sizeof(struct EthernetHeader) + sizeof(struct IpHeader), &icmp, sizeof(struct IcmpHeader));
    memcpy(SendBuffer + sizeof(struct EthernetHeader) + sizeof(struct IpHeader) + sizeof(struct IcmpHeader), IcmpData, strlen(IcmpData));
    memset(CheckBuffer, 0, sizeof(CheckBuffer));
    memcpy(CheckBuffer, &ip, sizeof(struct IpHeader));
    ip.Checksum = checksum((USHORT*)(CheckBuffer), sizeof(struct IpHeader));
    memcpy(SendBuffer + sizeof(struct EthernetHeader), &ip, sizeof(struct IpHeader));
    Result = pcap_sendpacket(WinpcapHandle, SendBuffer, sizeof(struct EthernetHeader) + sizeof(struct IpHeader) + sizeof(struct IcmpHeader) + strlen(IcmpData));
    if (Result != 0)
    {
        printf("Send Error!\n");
    } 
    else
    {
        printf("Send ICMP Packet.\n");
        printf("ICMP Echo Reply.\n");
        printf("Type:%d\n", icmp.Type);
        printf("Code:%d\n", icmp.Code);
        printf("Checksum:%d\n", ntohs(icmp.Checksum));
        printf("ID:%d\n", ntohs(icmp.ID));
        printf("Sequence:%d\n", ntohs(icmp.Sequence));
        printf("Icmp data:%s\n", IcmpData);
    }
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
