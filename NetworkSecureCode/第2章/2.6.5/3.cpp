//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include "winsock.h" 
#include <stdio.h>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
void main()
{
    IP_ADAPTER_INFO Interface[20];
    PIP_ADAPTER_INFO NetInterface = NULL;
    DWORD Result = 0;
    unsigned long Length = sizeof(Interface);
    Result = GetAdaptersInfo(Interface, &Length);
    if (Result != NO_ERROR)
    {
        printf("GetAdaptersInfo failed error.\n");
    }
    else
    {
        NetInterface = Interface;
        while (NetInterface)
        {
            static int number;
            number++;
            printf("第%d个网络接口信息:\n", number);
            printf("名称：%s\n", NetInterface->AdapterName);
            printf("信息：%s\n", NetInterface->Description);
            printf("地址：%ld\n", NetInterface->Address);
            printf("MAC地址:%02X:%02X:%02X:%02X:%02X:%02X\n", NetInterface->Address[0], NetInterface->Address[1], NetInterface->Address[2], NetInterface->Address[3], NetInterface->Address[4], NetInterface->Address[5]);
            printf("IP地址：%s\n", NetInterface->IpAddressList.IpAddress.String);
            printf("IP地址掩码:%s\n", NetInterface->IpAddressList.IpMask.String);
            printf("网关:%s\n", NetInterface->GatewayList.IpAddress.String);
            if (NetInterface->DhcpEnabled)
            {
                printf("DHCP配置：\n");
                printf("服务器:%s\n", NetInterface->DhcpServer.IpAddress.String);
                printf("租用:%ld\n", NetInterface->LeaseObtained);
            }
            else
                printf("无DHCP配置\n");
            if (NetInterface->HaveWins)
            {
                printf("Wins配置:\n");
                printf("主Wins服务器:%s\n", NetInterface->PrimaryWinsServer.IpAddress.String);
                printf("次Wins服务器:%s\n", NetInterface->SecondaryWinsServer.IpAddress.String);
            }
            else
                printf("无服务器配置\n");
            NetInterface = NetInterface->Next;
        }
    }
}
