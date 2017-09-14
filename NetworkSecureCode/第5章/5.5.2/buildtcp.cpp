//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <win32/libnet.h>
void main() 
{
    libnet_t *LibnetHandle;
    int PacketLength; 
    char *DestAddr = "192.168.1.65";
    char *SourAddr = "192.168.1.9";
    u_char SourMAC[6] = 
    {
        0x31, 0x52, 0x23, 0x64, 0x5a, 0xb6
    };
    u_char DestMAC[6] = 
    {
        0xac, 0xbc, 0x8f, 0xc3, 0x9c, 0x5c
    };
    u_long DestAddrNumber;
    u_long SourAddrNumber;
    char LibnetError[LIBNET_ERRBUF_SIZE];
    u_short SourPort;
    u_short DestPort; 
    libnet_ptag_t LibnetPtag;
    char TcpData[] = "Tcp Content.";
    u_long TcpDataLength = 0;
    pcap_if_t *NetwokDevice;
    char WinpcapError[PCAP_ERRBUF_SIZE];
    pcap_if_t *Device;
    char DeviceName[100][1000];
    int DeviceIndex = 0;
    int Result;
    int i;
    TcpDataLength = strlen(TcpData);
    SourPort = 666;
    DestPort = 80;
    Result = pcap_findalldevs(&NetwokDevice, WinpcapError);
    if (Result ==  - 1)
    {
        printf("pcap_findalldevs Error");
        return ;
    }
    for (Device = NetwokDevice, i = 0; Device && i < 10; Device = Device->next, i++)
    {
        printf("Number %d:", i);
        printf("%s:", Device->name);
        printf("%s\n", Device->description);
        sprintf(DeviceName[i], "%s", Device->name);
    }
    pcap_freealldevs(NetwokDevice);
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
    LibnetHandle = libnet_init(LIBNET_LINK, DeviceName[DeviceIndex], LibnetError);
    if (LibnetHandle == NULL)
    {
        printf("libnet_init failed with error %s\n", LibnetError);
        exit(0);
    }
    DestAddrNumber = libnet_name2addr4(LibnetHandle, DestAddr, LIBNET_RESOLVE);
    SourAddrNumber = libnet_name2addr4(LibnetHandle, SourAddr, LIBNET_RESOLVE);
    LibnetPtag = libnet_build_tcp(SourPort, DestPort, 12, 13, TH_SYN, 235, 0, 0, LIBNET_TCP_H + strlen(TcpData), (unsigned char*)TcpData, TcpDataLength, LibnetHandle, 0);
    if (LibnetPtag ==  - 1)
    {
        printf("libnet_build_tcp failed with error %s\n", libnet_geterror(LibnetHandle));
    }
    LibnetPtag = libnet_build_ipv4(LIBNET_IPV4_H + LIBNET_TCP_H + TcpDataLength, 0, 242, 0, 64, IPPROTO_TCP, 0, SourAddrNumber, DestAddrNumber, NULL, 0, LibnetHandle, 0);
    if (LibnetPtag ==  - 1)
    {
        printf("libnet_build_tcp failed with error %s\n", libnet_geterror(LibnetHandle));
    }
    LibnetPtag = libnet_build_ethernet(DestMAC, SourMAC, ETHERTYPE_IP, NULL, 0, LibnetHandle, 0);
    if (LibnetPtag ==  - 1)
    {
        printf("libnet_build_tcp failed with error %s\n", libnet_geterror(LibnetHandle));
    }
    PacketLength = libnet_write(LibnetHandle);
    if (PacketLength ==  - 1)
    {
        printf("libnet_write failed with error %s\n", libnet_geterror(LibnetHandle));
    }
    else
    {
        printf("Send Tcp Data.\n");
    }
    libnet_destroy(LibnetHandle);
}
