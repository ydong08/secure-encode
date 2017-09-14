#ifndef _RAWSOCKET_H
#define _RAWSOCKET_H

class rawsocket
{
    private:
	int sockfd;
    public:
	rawsocket(const int protocol);
	~rawsocket() ;

	//set the promiscuous mode.
	bool dopromisc(char *nif);
	
	//capture packets.
	int receive(char *recvbuf,int buflen,struct sockaddr_in *from,int *addrlen);
};
#endif
