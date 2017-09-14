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
    int Result;
    char SendBuffer[200];
    char IpData[] = "Ip Data Test.";
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
    memcpy(&SendBuffer, &ethernet, 14);
    ip.Version_HLen = 0x45;
    ip.TOS = 0;
    ip.Length = htons(sizeof(struct IpHeader) + strlen(IpData));
    ip.Ident = htons(1);
    ip.Flags_Offset = 0;
    ip.TTL = 128;
    ip.Protocol = 6;
    ip.Checksum = 0;
    ip.SourceAddr = inet_addr("192.168.1.9");
    ip.DestinationAddr = inet_addr("192.168.1.10");
    memcpy(&SendBuffer[14], &ip, 20);
    memcpy(&SendBuffer[14+20], IpData, strlen(IpData));
    ip.Checksum = checksum((USHORT*) &SendBuffer[14], 20);
    memcpy(&SendBuffer[14], &ip, 20);
    Result = pcap_sendpacket(WinpcapHandle, (const unsigned char*)SendBuffer, 14+20+strlen(IpData));
    if (Result != 0)
    {
        printf("Send Error!\n");
    } 
    else
    {
        printf("Send IP Packet.\n");
        printf("Version:%d\n", ip.Version_HLen >> 4);
        printf("Herader Length:%d\n", (ip.Version_HLen &0x0f) *4);
        printf("TOS:%d\n", ip.TOS);
        printf("Length:%d\n", ntohs(ip.Length));
        printf("Ident:%d\n", ntohs(ip.Ident));
        printf("Flags_Offset:%d\n", ntohs(ip.Flags_Offset));
        printf("TTL:%d\n", ip.TTL);
        printf("Protocol:%d\n", ip.Protocol);
        printf("Checksum:%u\n", ntohs(ip.Checksum));
        SOCKADDR_IN ipaddr;
        ipaddr.sin_addr.s_addr = ip.SourceAddr;
        printf("Source IP:%s\n", inet_ntoa(ipaddr.sin_addr));
        ipaddr.sin_addr.s_addr = ip.DestinationAddr;
        printf("Destination IP:%s\n", inet_ntoa(ipaddr.sin_addr));
    }
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
