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
struct ArpHeader
{
    u_short HardwareType;
    u_short ProtocolType;
    u_char MACLength;
    u_char IpAddLength;
    u_short OperationCode;
    u_char SourceMAC[6];
    unsigned char SourceIP[4];
    u_char DestinationMAC[6];
    unsigned char DestinationIP[4];
};
int main(int argc, char **argv)
{
    struct EthernetHeader ethernet;
    struct ArpHeader arp;
    int Result;
    unsigned char SendBuffer[200];
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
    memset(ethernet.DestMAC, 0xff, 6);
    BYTE hostmac[8];
    hostmac[0] = 0x00;
    hostmac[1] = 0x1a;
    hostmac[2] = 0x4d;
    hostmac[3] = 0x70;
    hostmac[4] = 0xa3;
    hostmac[5] = 0x89;
    memcpy(ethernet.SourMAC, hostmac, 6);
    ethernet.EthType = htons(0x0806);
    memcpy(&SendBuffer, &ethernet, 14);
    arp.HardwareType = htons(0x0001);
    arp.ProtocolType = htons(0x0800);
    arp.MACLength = 6;
    arp.IpAddLength = 4;
    arp.OperationCode = htons(0x0001);
    memcpy(arp.SourceMAC, ethernet.SourMAC, 6);
    unsigned char hostip[4];
    hostip[0] = 192;
    hostip[1] = 168;
    hostip[2] = 1;
    hostip[3] = 9;
    memcpy(&arp.SourceIP, hostip, 4);
    memset(arp.DestinationMAC, 0, 6);
    unsigned char mydestip[4];
    mydestip[0] = 192;
    mydestip[1] = 168;
    mydestip[2] = 1;
    mydestip[3] = 6;
    memcpy(&arp.DestinationIP, mydestip, 4);
    memcpy(&SendBuffer[14], &arp, 28);
    Result = pcap_sendpacket(WinpcapHandle, SendBuffer, 14+28);
    if (Result != 0)
    {
        printf("Send Error!\n");
    }
    else
    {
        printf("Send ARP Packet.\n");
        printf("HardwareType:%d\n", ntohs(arp.HardwareType));
        printf("ProtocolType:%d\n", ntohs(arp.ProtocolType));
        printf("MACLength:%d\n", arp.MACLength);
        printf("IpAddLength:%d\n", arp.IpAddLength);
        printf("OperationCode:%d\n", ntohs(arp.OperationCode));
        printf("SourceMAC:%02x:%02x:%02x:%02x:%02x:%02x\n", arp.SourceMAC[0], arp.SourceMAC[1], arp.SourceMAC[2], arp.SourceMAC[3], arp.SourceMAC[4], arp.SourceMAC[5]);
        printf("SourceIP:%d.%d.%d.%d\n", arp.SourceIP[0], arp.SourceIP[1], arp.SourceIP[2], arp.SourceIP[3]);
        printf("DestinationMac:%02x:%02x:%02x:%02x:%02x:%02x\n", arp.DestinationMAC[0], arp.DestinationMAC[1], arp.DestinationMAC[2], arp.DestinationMAC[3], arp.DestinationMAC[4], arp.DestinationMAC[5]);
        printf("DestinationIP:%d.%d.%d.%d\n", arp.DestinationIP[0], arp.DestinationIP[1], arp.DestinationIP[2], arp.DestinationIP[3]);
    }
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
