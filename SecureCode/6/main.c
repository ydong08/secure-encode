#include "rawsocsniffer.h"
//#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
int main(int argc, char ** argv)
{
    //rawsocsniffer sniffer(htons(ETH_P_ALL));
    rawsocsniffer sniffer(htons(0x0003));
    char ch;
    filter myfilter;
    memset(&myfilter,0,sizeof(myfilter));
    while((ch=getopt(argc, argv,"s:d:atui"))!=-1)
    {
	switch (ch)
	{
	    case 's':
		myfilter.sip=inet_addr(optarg);
		break;
	    case 'd':
		myfilter.dip=inet_addr(optarg);
		break;
	    case 'a':
		sniffer.setbit((myfilter.protocol),1);
		break;
	    case 't':
		sniffer.setbit((myfilter.protocol),2);
		break;
	    case 'u':
		sniffer.setbit((myfilter.protocol),3);
		break;
	    case 'i':
		sniffer.setbit((myfilter.protocol),4);
		break;
	    default:
		break;
	}
    }
    cout<<"create sniffer succeed."<<endl;
    
    //set sniffer filter;
    sniffer.setfilter(myfilter);
    
    //sniffer initialize
    if(!sniffer.init())
    {
	cout<<"sniffer initialize error!"<<endl;
	exit(-1);
    }

    //start to capture packets;
    sniffer.sniffer();
}
