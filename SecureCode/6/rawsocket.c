#include <iostream>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>   
#include <netinet/ip.h>  
#include <netinet/tcp.h>  
#include <netinet/udp.h> 
#include <unistd.h>  
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <net/ethernet.h>
#include <string.h>
#include "rawsocket.h"

rawsocket::rawsocket(const int protocol)
{
    sockfd=socket(PF_PACKET,SOCK_RAW,protocol);
    if(sockfd<0)
    {
	perror("socket error: ");
    }
}


rawsocket::~rawsocket()
{
    close(sockfd);
}


bool rawsocket::dopromisc(char*nif)
{
    struct ifreq ifr;              
    strncpy(ifr.ifr_name, nif,strlen(nif)+1);  
    if((ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1))  
    {         
       	perror("ioctlread: ");  
	return false;
    }	
    ifr.ifr_flags |= IFF_PROMISC; 
    if(ioctl(sockfd, SIOCSIFFLAGS, &ifr) == -1 )
    { 
     	perror("ioctlset: ");
	return false;
    }

    return true;
}

int rawsocket::receive(char *recvbuf,int buflen, struct sockaddr_in *from,int *addrlen)
{
    int recvlen;
    recvlen=recvfrom(sockfd,recvbuf,buflen,0,(struct sockaddr *)from,(socklen_t *)addrlen);
    recvbuf[recvlen]='\0';
    return recvlen;
}

