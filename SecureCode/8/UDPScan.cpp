#include "UDPScan.h"
//=============================================================================
void UDPScanHost(struct UDPScanHostThrParam *p)
{
	string HostIP;
	unsigned HostPort,LocalPort,LocalHostIP;

	int UDPSock;
	struct sockaddr_in UDPScanHostAddr,FromHostAddr; 
	int n,FromLen;
	int on,ret;

	struct icmp *icmp;
	struct ipicmphdr hdr;

	struct iphdr *ip; 
    struct udphdr *udp; 
    struct pseudohdr *pseudo; 
    char packet[sizeof(struct iphdr)+sizeof(struct udphdr)];

	char SendBuf[2];//[100];
	char RecvBuf[100];

	int HeadLen;
	struct   timeval TimeOut;

	struct timeval TpStart,TpEnd;      //for time of receive udp response
	float TimeUse;

	//-----------------Get IP and Port--------------------------------
    HostIP = p->HostIP;
	HostPort = p->HostPort;
	LocalPort = p->LocalPort;
	LocalHostIP = p->LocalHostIP;

	//-----------------Great Raw socket-----------------------------------------
	UDPSock=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP); 
    if(UDPSock < 0)
	{
		pthread_mutex_lock(&UDPPrintlocker);
		cout<<"Can't creat raw icmp socket !"<<endl;
		pthread_mutex_unlock(&UDPPrintlocker);
	}
    
	//-----------------Set socket option-----------------------------------------
	on = 1;
    ret = setsockopt(UDPSock,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on)); 
	if (ret < 0) 
	{
		pthread_mutex_lock(&UDPPrintlocker);
		cout<<"Can't set raw socket !"<<endl;
		pthread_mutex_unlock(&UDPPrintlocker);
	}

    //-----------------set UDP Scan Host address--------------------------------
	memset(&UDPScanHostAddr,0,sizeof(UDPScanHostAddr));
	UDPScanHostAddr.sin_family = AF_INET;
	UDPScanHostAddr.sin_addr.s_addr = inet_addr(&HostIP[0]);
	UDPScanHostAddr.sin_port = htons(HostPort); 
	    
	//-------------------------fill packet------------------------------------------
	memset(packet, 0x00, sizeof(packet)); 

	ip = (struct iphdr *)packet; 
    udp = (struct udphdr *)(packet + sizeof(struct iphdr)); 
    pseudo = (struct pseudohdr *)(packet + sizeof(struct iphdr) - sizeof(struct pseudohdr)); 

	//-----------------fill udp packet-----------------------------------------
	udp->source = htons(LocalPort);                 
    udp->dest = htons(HostPort);           
    udp->len = htons(sizeof(struct udphdr));
    udp->check = 0;  

    //-----------------fill pseudo----------------------------------------------
	pseudo->saddr = LocalHostIP; 
    pseudo->daddr = inet_addr(&HostIP[0]); 
    pseudo->useless = 0; 
    pseudo->protocol = IPPROTO_UDP; 
    pseudo->length = udp->len; 

	udp->check = in_cksum((u_short *)pseudo,sizeof(struct udphdr)+sizeof(struct pseudohdr));
	
	//-----------------fill ip packet-----------------------------------------
	ip->ihl = 5; 
	ip->version = 4; 
	ip->tos = 0x10; 
	ip->tot_len = sizeof(packet); 
	ip->frag_off = 0; 
	ip->ttl = 69; 
	ip->protocol = IPPROTO_UDP; 
	ip->check = 0; 
	ip->saddr = inet_addr("192.168.1.168"); 
	ip->daddr = inet_addr(&HostIP[0]);

    //-----------------send udp packet-----------------------------------------
	n = sendto(UDPSock, packet, ip->tot_len, 0, (struct sockaddr *)&UDPScanHostAddr, sizeof(UDPScanHostAddr)); 
	if (n<0) 
	{
		pthread_mutex_lock(&UDPPrintlocker);
		cout<<"Send message to Host Failed !"<<endl;
		pthread_mutex_unlock(&UDPPrintlocker);
	}
	
	//-----------------set socket in non-blocked model-------------------------
	if(fcntl(UDPSock, F_SETFL, O_NONBLOCK) == -1) 
	{
		pthread_mutex_lock(&UDPPrintlocker);
        cout<<"Set socket in non-blocked model fail !"<<endl;
		pthread_mutex_unlock(&UDPPrintlocker);
	}
    
	//--------------------------receive circle----------------------------------
	gettimeofday(&TpStart,NULL);             //get start time
	do 
	{
		//receive response message
        n = read(UDPSock, (struct ipicmphdr *)&hdr, sizeof(hdr));

		if(n > 0)
		{
			if((hdr.ip.saddr == inet_addr(&HostIP[0])) && (hdr.icmp.code == 3) && (hdr.icmp.type == 3))
			{
				pthread_mutex_lock(&UDPPrintlocker);
				cout<<"Host: "<<HostIP<<" Port: "<<HostPort<<" closed !"<<endl;
				pthread_mutex_unlock(&UDPPrintlocker);
				break;
			}
		}
		//time out?
		gettimeofday(&TpEnd,NULL);
		TimeUse=(1000000*(TpEnd.tv_sec-TpStart.tv_sec)+(TpEnd.tv_usec-TpStart.tv_usec))/1000000.0;
		if(TimeUse<3)
		{
			continue;
		}
		else
		{
			pthread_mutex_lock(&UDPPrintlocker);
			cout<<"Host: "<<HostIP<<" Port: "<<HostPort<<" open !"<<endl;
			pthread_mutex_unlock(&UDPPrintlocker);
			break;
		}
	} 
	while(true);

    //close socket
	close(UDPSock);
	delete p;
}

//==============================================================================
void* Thread_UDPScan(void* param)
{
    struct UDPThrParam *p;

	string HostIP;
	unsigned BeginPort,EndPort,TempPort,LocalPort,LocalHostIP;

	pthread_t subThreadID;
	pthread_attr_t attr;
	int ret;
    
	p=(struct UDPThrParam*)param;
	HostIP = p->HostIP;
	BeginPort = p->BeginPort;
	EndPort = p->EndPort;
	LocalHostIP = p->LocalHostIP;
	
	
	//---------------receive circle for scaning from BeginPort to EndPort--------------------------

	LocalPort = 1024;
 
	for (TempPort=BeginPort;TempPort<=EndPort;TempPort++) 
	{
        UDPScanHostThrParam *pUDPScanHostParam = new UDPScanHostThrParam;
        pUDPScanHostParam->HostIP = HostIP;
		pUDPScanHostParam->HostPort = TempPort;
        pUDPScanHostParam->LocalPort = TempPort + LocalPort;
        pUDPScanHostParam->LocalHostIP = LocalHostIP;

		UDPScanHost(pUDPScanHostParam);

	}
    //---------------exit thread----------------------
    cout<<"UDP Scan thread exit !"<<endl;
	pthread_exit(NULL);
}

