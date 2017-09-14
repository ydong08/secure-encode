//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "stdio.h"
#include "pcap.h"
#pragma comment(lib,"wpcap.lib") 
#pragma comment(lib,"WS2_32.lib") 
int main()
{
    int Result, i;
    char PcapError[PCAP_ERRBUF_SIZE];
    pcap_if_t *Interface;
    pcap_if_t *NetInterface;
    Result = pcap_findalldevs(&NetInterface, PcapError);
    if (Result ==  - 1 || NetInterface == NULL)
        return FALSE;
    for (Interface = NetInterface, i = 0; Interface && i < 10; Interface = Interface->next, i++)
    {
        pcap_addr_t *a;
        printf("第%d个网络接口:\n", i + 1);
        printf("名称：%s\n", Interface->name);
        if (Interface->description)
            printf("信息：%s\n", Interface->description);
        for (a = Interface->addresses; a; a = a->next)
        {
            switch (a->addr->sa_family)
            {
                case AF_INET:
                    if (a->addr)
                        printf("IP地址:%s\n", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
                    if (a->netmask)
                        printf("地址掩码：%s\n", inet_ntoa(((struct sockaddr_in*)a->netmask)->sin_addr));
                    break;
                default:
                    break;
            }
        }
    }
    pcap_freealldevs(NetInterface);
    return 1;
}
