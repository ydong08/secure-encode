//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "pcap.h" 
#pragma comment(lib,"wpcap.lib") 
#pragma comment(lib,"WS2_32.lib") 
void CallbackFunctionPacket(unsigned char *user, const struct pcap_pkthdr *winpcaphead, const unsigned char *packetdata)
{
    static int PacketNumber = 0;
    PacketNumber++;
    printf("\nPacketNumber:%d\n", PacketNumber);
    printf("PacketData:\n");
    int IpDataLength = winpcaphead->len;
    char IpDataOut[65535] = 
    {
        0
    };
    int end = 0;
    if (IpDataLength > 0)
    {
        for (int i = 0; i < IpDataLength; i++)
        {
            printf("%02X", packetdata[i]);
            if (isgraph(packetdata[i]))
                IpDataOut[end] = packetdata[i];
            else if (packetdata[i] == ' ')
                IpDataOut[end] = packetdata[i];
            else
                IpDataOut[end] = '.';
            end = end + 1;
            if (i % 16 == 15)
            {
                IpDataOut[end] = 0;
                printf("   %s", IpDataOut);
                end = 0;
                printf("\n");
            }
        }
        if (end > 0)
        {
            for (int k = end * 2; k < 32; k++)
            {
                printf(" ");
            }
            IpDataOut[end] = 0;
            printf("   %s", IpDataOut);
            printf("\n");
        }
    }
}

int main(int argc, char **argv)
{
    int Result;
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
    }
    for (Device = NetwokDevice, i = 0; Device && i < 10; Device = Device->next, i++)
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
    for (Device = NetwokDevice, i = 0; i < DeviceIndex; Device = Device->next, i++)
        ;
    WinpcapHandle = pcap_open_live(LPCTSTR(DeviceName[DeviceIndex]), 65535, 1, 1000, WinpcapError);
    if (WinpcapHandle == NULL)
    {
        printf("pcap_open_live error");
        pcap_freealldevs(NetwokDevice);
        return 0;
    }
    struct in_addr ip;
    char name[200];
    HOSTENT *host;
    gethostname(name, 200);
    host = gethostbyname(name);
    memcpy(&ip, host->h_addr_list[0], host->h_length);
    char Rule[300] = "icmp";
    u_int netmask;
    struct bpf_program fcode;
    if (Device->addresses != NULL)
        netmask = ((struct sockaddr_in*)(Device->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        netmask = 0xffffffff;
    Result = pcap_compile(WinpcapHandle, &fcode, Rule, 1, netmask);
    if (Result < 0)
    {
        printf("pcap_compile failed\n");
        return 0;
    } Result = pcap_setfilter(WinpcapHandle, &fcode);
    if (Result < 0)
    {
        printf("pcap_setfilter failed\n");
    }
    pcap_loop(WinpcapHandle,  - 1, CallbackFunctionPacket, NULL);
    pcap_freealldevs(NetwokDevice);
    pcap_close(WinpcapHandle);
    return 1;
}
