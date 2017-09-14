#include "Scaner.h"

//====================================================================================
int main(int argc,char* argv[])
{
    string HostIP;
	unsigned BeginPort,EndPort,LocalHostIP;
	int ret;

	struct TCPConThrParam TCPConParam;
	struct UDPThrParam UDPParam;
	struct TCPSYNThrParam TCPSynParam;
	struct TCPFINThrParam TCPFinParam;
	
	pthread_t ThreadID;

	char* pTcpCon = {"-c"};           // command for tcp connect scan
    char* pTcpSyn = {"-s"};           // command for tcp syn scan
	char* pTcpFin = {"-f"};           // command for tcp fin scan
	char* pUdp = {"-u"};              // command for udp scan
	char* pHelp = {"-h"};             // command for help

	
	//-----------------------Is param OK-------------------------------
	if (argc != 2) 
	{
		cout<<"Parameter error !"<<endl;
		return -1;
	}

	//-----------------------TCP connect Scan--------------------------
	if (!strcmp(pHelp,argv[1]))
	{
		cout<<"Scaner: usage: [-h]  --help information"<<endl;
		cout<<"               [-c]  --TCP connect scan"<<endl;
        cout<<"               [-s]  --TCP syn scan"<<endl;
	    cout<<"               [-f]  --TCP fin scan"<<endl;
		cout<<"               [-u]  --UDP scan"<<endl;
        return 1;
	}

    //--------------------Input IP address----------------------------
	cout<<"Please input IP address of a Host:";
	cin>>HostIP;

        
    if(inet_addr(&(HostIP[0])) == INADDR_NONE)
	{
            cout<<"IP address wrong!"<<endl;
            return -1;
	}
   
    //--------------------Input the range of port---------------------
	cout<<"Please input the range of port..."<<endl;
	cout<<"Begin Port:";
	cin>>BeginPort;
	cout<<"End Port:";
	cin>>EndPort;
	
	if(IsPortOK(BeginPort,EndPort))
	{
		cout<<"Scan Host "<<HostIP<<" port "<<BeginPort<<"~"<<EndPort<<" ..."<<endl;
	}
	else
	{
		cout<<"The range of port is wrong !"<<endl;
                return -1;
	}
	
	//----------------------Get Local Host IP-------------------------
	LocalHostIP = GetLocalHostIP();
	
	//-----------------------Ping Scan Host---------------------------
	if (Ping(HostIP,LocalHostIP) == false) 
	{
		cout<<"Ping Host "<<HostIP<<" failed, stop scan it !"<<endl;
		return -1;
	}
 
	//-----------------------TCP connect Scan---------------------------
	if (!strcmp(pTcpCon,argv[1]))
	{
		cout<<"Begin TCP connect scan..."<<endl;
		//create thread for TCP connect scan
        TCPConParam.HostIP = HostIP;
        TCPConParam.BeginPort = BeginPort;
        TCPConParam.EndPort = EndPort;
        ret = pthread_create(&ThreadID,NULL,Thread_TCPconnectScan,&TCPConParam);
	    if (ret==-1) 
		{
			cout<<"Can't create the TCP connect scan thread !"<<endl;
			return -1;
		}
		ret = pthread_join(ThreadID,NULL);
		if(ret != 0)
		{
			cout<<"call pthread_join function failed !"<<endl;
			return -1;
		}
		else
		{
			cout<<"TCP Connect Scan finished !"<<endl;
			return 0;
		}
	}
	//-----------------------TCP syn Scan---------------------------
	if (!strcmp(pTcpSyn,argv[1]))
	{
		cout<<"Begin TCP SYN scan..."<<endl;
		//create thread for TCP SYN scan
        TCPSynParam.HostIP = HostIP;
        TCPSynParam.BeginPort = BeginPort;
        TCPSynParam.EndPort = EndPort;
	    TCPSynParam.LocalHostIP = LocalHostIP;
        ret = pthread_create(&ThreadID,NULL,Thread_TCPSynScan,&TCPSynParam);
	    if (ret==-1) 
		{
			cout<<"Can't create the TCP SYN scan thread !"<<endl;
			return -1;
		}

		ret = pthread_join(ThreadID,NULL);
		if(ret != 0)
		{
			cout<<"call pthread_join function failed !"<<endl;
			return -1;
		}
		else
		{
			cout<<"TCP SYN Scan finished !"<<endl;
			return 0;
		}
	}
	//-----------------------TCP fin Scan---------------------------
	if (!strcmp(pTcpFin,argv[1]))
	{
		cout<<"Begin TCP FIN scan..."<<endl;
		//create thread for TCP FIN scan
        TCPFinParam.HostIP = HostIP;
        TCPFinParam.BeginPort = BeginPort;
        TCPFinParam.EndPort = EndPort;
	    TCPFinParam.LocalHostIP = LocalHostIP;
        ret = pthread_create(&ThreadID,NULL,Thread_TCPFinScan,&TCPFinParam);
	    if (ret==-1) 
		{
			cout<<"Can't create the TCP FIN scan thread !"<<endl;
			return -1;
		}

		ret = pthread_join(ThreadID,NULL);
		if(ret != 0)
		{
			cout<<"call pthread_join function failed !"<<endl;
			return -1;
		}
		else
		{
			cout<<"TCP FIN Scan finished !"<<endl;
			return 0;
		}
	}
	//-----------------------UDP Scan-------------------------------
    if (!strcmp(pUdp,argv[1]))
	{
		cout<<"Begin UDP scan..."<<endl;
		//create thread for UDP scan
        UDPParam.HostIP = HostIP;
        UDPParam.BeginPort = BeginPort;
        UDPParam.EndPort = EndPort;
		UDPParam.LocalHostIP = LocalHostIP;
        ret = pthread_create(&ThreadID,NULL,Thread_UDPScan,&UDPParam);
		if (ret==-1) 
		{
			cout<<"Can't create the UDP scan thread !"<<endl;
			return -1;
		}

		ret = pthread_join(ThreadID,NULL);
		if(ret != 0)
		{
			cout<<"call pthread_join function failed !"<<endl;
			return -1;
		}
		else
		{
			cout<<"UDP Scan finished !"<<endl;
			return 0;
		}
	}
}

