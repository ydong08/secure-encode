#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <net/if.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> 
#include <memory.h>
#include <sys/ioctl.h>
#include "des.h"

using namespace std;

#define SERVERPORT 9527
#define BUFFERSIZE 32	
char strStdinBuffer[BUFFERSIZE];
char strSocketBuffer[BUFFERSIZE];
char strEncryBuffer[BUFFERSIZE];
char strDecryBuffer[BUFFERSIZE];

void SecretChat(int nSock,char *pRemoteName, char *pKey)
{
	CDesOperate cDes;
	if(strlen(pKey)!=8)
	{
		printf("Key length error");
		return ;
	}
	
	pid_t nPid;
	nPid = fork();
	if(nPid != 0)
	{
		while(1)
		{
			bzero(&strSocketBuffer, BUFFERSIZE);
			int nLength = 0;
			nLength = recv(nSock, strSocketBuffer,BUFFERSIZE,0);
			if(nLength !=BUFFERSIZE)
			{
				break;
			}
			else
			{
				int nLen = BUFFERSIZE;
				cDes.Decry(strSocketBuffer,BUFFERSIZE,strDecryBuffer,nLen,pKey,8);
				strDecryBuffer[BUFFERSIZE-1]=0;
				if(strDecryBuffer[0]!=0&&strDecryBuffer[0]!='\n')
				{
					printf("Receive message form <%s>: %s\n", pRemoteName,strDecryBuffer);
					if(0==memcmp("quit",strDecryBuffer,4))
					{
						printf("Quit!\n");
						break;
					}
				}
			}
		}
	}
	else
	{ 
		while(1)
		{
			bzero(&strStdinBuffer, BUFFERSIZE);
			while(strStdinBuffer[0]==0)
			{
				if (fgets(strStdinBuffer, BUFFERSIZE, stdin) == NULL) 
				{
					continue;
				}
			}      
			int nLen = BUFFERSIZE;
			cDes.Encry(strStdinBuffer,BUFFERSIZE,strEncryBuffer,nLen,pKey,8);
			if(send(nSock, strEncryBuffer, BUFFERSIZE,0)!=BUFFERSIZE)
			{
				perror("send");
			}
			else
			{
				if(0==memcmp("quit",strStdinBuffer,4))
				{
					printf("Quit!\n");
					break;
				}
			}
		}
	}		

}

int main(int argc,char * [])
{

	printf("Client or Server?\r\n");
	cin>>strStdinBuffer;
	if(strStdinBuffer[0]=='c'||strStdinBuffer[0]=='C')
	{//be a client
		char strIpAddr[16];
		printf("Please input the server address:\r\n");
		cin>>strIpAddr;

		int nConnectSocket, nLength;
		struct sockaddr_in sDestAddr;
		if ((nConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{
			perror("Socket");
			exit(errno);
		}

		bzero(&sDestAddr, sizeof(sDestAddr));
		sDestAddr.sin_family = AF_INET;
		sDestAddr.sin_port = htons(SERVERPORT);
		sDestAddr.sin_addr.s_addr = inet_addr(strIpAddr);
          /* 连接服务器 */
		if (connect(nConnectSocket, (struct sockaddr *) &sDestAddr, sizeof(sDestAddr)) != 0) 
		{
			perror("Connect ");
			exit(errno);
		}
		else
		{
			printf("Connect Success!  \nBegin to chat...\n");
			SecretChat(nConnectSocket,strIpAddr,"benbenmi");
		}
        close(nConnectSocket);		
	}
	else
	{//be a server
		int nListenSocket, nAcceptSocket;
		socklen_t nLength = 0;
		struct sockaddr_in sLocalAddr, sRemoteAddr;
		if ((nListenSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
		{
			perror("socket");
			exit(1);
		}
		
		bzero(&sLocalAddr, sizeof(sLocalAddr));
		sLocalAddr.sin_family = PF_INET;
		sLocalAddr.sin_port = htons(SERVERPORT);
		sLocalAddr.sin_addr.s_addr = INADDR_ANY;
		
		if (bind(nListenSocket, (struct sockaddr *) &sLocalAddr, sizeof(struct sockaddr))== -1)
		{
			perror("bind");
			exit(1);
		}
		if (listen(nListenSocket, 5) == -1) 
		{
			perror("listen");
			exit(1);
		}
		printf("Listening...\n");
		nLength = sizeof(struct sockaddr);
		if ((nAcceptSocket = accept(nListenSocket, (struct sockaddr *) &sRemoteAddr,&nLength)) == -1) 
		{
			perror("accept");
			exit(errno);
		} 
		else
		{
			close(nListenSocket);
			printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa(sRemoteAddr.sin_addr),ntohs(sRemoteAddr.sin_port), nAcceptSocket);
			SecretChat(nAcceptSocket,inet_ntoa(sRemoteAddr.sin_addr),"benbenmi");
			close(nAcceptSocket);
			
		}
	}
}
