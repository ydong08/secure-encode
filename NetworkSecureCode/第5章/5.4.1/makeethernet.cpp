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
int main(int argc, char **argv)
{
    struct EthernetHeader ethernet;
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
    BYTE destmac[8];
    destmac[0] = 0x00;
    destmac[1] = 0x11;
    destmac[2] = 0x22;
    destmac[3] = 0x33;
    destmac[4] = 0x44;
    destmac[5] = 0x55;
    memcpy(ethernet.DestMAC, destmac, 6);
    BYTE srcmac[8];
    srcmac[0] = 0x12;
    srcmac[1] = 0x34;
    srcmac[2] = 0x56;
    srcmac[3] = 0x78;
    srcmac[4] = 0x90;
    srcmac[5] = 0xab;
    memcpy(ethernet.SourMAC, srcmac, 6);
    ethernet.EthType = htons(0x0800);
    memcpy(&SendBuffer, &ethernet, 14);
    char EthernetData[] = "Ethernet Data";
    memcpy(&SendBuffer[14], EthernetData, strlen(EthernetData));
    Result = pcap_sendpacket(WinpcapHandle, SendBuffer, 14+strlen(EthernetData));
    if (Result != 0)
    {
        printf("Send Error!\n");
    }
    else
    {
        printf("Send Ethernet Packet.\n");
        printf("DestinationMAC:%02x:%02x:%02x:%02x:%02x:%02x\n", ethernet.DestMAC[0], ethernet.DestMAC[1], ethernet.DestMAC[2], ethernet.DestMAC[3], ethernet.DestMAC[4], ethernet.DestMAC[5]);
        printf("SourceMAC:%02x:%02x:%02x:%02x:%02x:%02x\n", ethernet.SourMAC[0], ethernet.SourMAC[1], ethernet.SourMAC[2], ethernet.SourMAC[3], ethernet.SourMAC[4], ethernet.SourMAC[5]);
        printf("Ethernet Type:0x%04x\n", ntohs(ethernet.EthType));
    }
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
