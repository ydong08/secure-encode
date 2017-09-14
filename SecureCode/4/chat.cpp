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
#include<time.h>
#include<stdlib.h>
#include <aio.h>
#include <semaphore.h> 

#include "des.h"
#include "rsa.h"

#define SERVERPORT 8435
#define BUFFERSIZE 32
#define DESKEYLENGTH 8

using namespace std;

char strStdinBuffer[BUFFERSIZE];
char strSocketBuffer[BUFFERSIZE];
char strEncryBuffer[BUFFERSIZE];
char strDecryBuffer[BUFFERSIZE];


char strStdoutBuffer[BUFFERSIZE*2];


char strDesKey[DESKEYLENGTH];
void GerenateDesKey(char *pKey)
{
	srand((unsigned long)time(NULL));
	for(int i=0;i<DESKEYLENGTH;i++)
	{
		pKey[i] = rand()%128;
	}
}

ssize_t TotalRecv(int s, void *buf, size_t len, int flags)
{
	size_t nCurSize = 0;
	while(nCurSize <len)
	{
		ssize_t nRes = recv(s,((char*)buf)+nCurSize,len-nCurSize,flags);
		if(nRes<0||nRes+nCurSize>len)
		{
			return -1;
		}
		nCurSize+=nRes;
	}
	return nCurSize;
}
#define ASYN

#ifdef NORMAL
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
			nLength = TotalRecv(nSock, strSocketBuffer,BUFFERSIZE,0);
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

#endif

#ifdef SELECT
void SecretChat(int nSock,char *pRemoteName, char *pKey)
{
	CDesOperate cDes;
	if(strlen(pKey)!=8)
	{
		printf("Key length error");
		return ;
	}
	fd_set cHandleSet; 
	struct timeval tv;
	int nRet;


	while(1)
	{	
		FD_ZERO(&cHandleSet); 
		FD_SET(nSock, &cHandleSet); 
		FD_SET(0, &cHandleSet);
		tv.tv_sec = 1;
		tv.tv_usec = 0; 
		nRet = select(nSock>0? nSock+ 1:1, &cHandleSet, NULL, NULL, &tv);
		if(nRet< 0)
		{
			printf("Select ERROR!\n");
			break;
		}
		if(0==nRet)
		{

			continue;
		}
		if(FD_ISSET(nSock,&cHandleSet))
		{
			bzero(&strSocketBuffer, BUFFERSIZE);
			int nLength = 0;
			nLength = TotalRecv(nSock, strSocketBuffer,BUFFERSIZE,0);
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
	
		if(FD_ISSET(0,&cHandleSet))
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
#endif



#ifdef ASYN

class SockoutSingle
{
public:
	char m_strKey[9];
	int m_nSock;
	aiocb *m_pReq;
	sem_t &m_bStop;
	SockoutSingle(int nSock,char *pKey,sem_t &bStop)
		:m_bStop(bStop)
	{
		memcpy(m_strKey,pKey,8);
		m_strKey[8]=0;
		this->m_nSock=nSock;
		this->m_pReq = new aiocb;
		bzero( (char *)m_pReq, sizeof(struct aiocb) );
		m_pReq->aio_fildes = nSock;
		m_pReq->aio_buf = strEncryBuffer;
		m_pReq->aio_nbytes = BUFFERSIZE;
		m_pReq->aio_offset = 0;
		m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._function =	SockoutReadCompletionHandler;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;
		m_pReq->aio_sigevent.sigev_value.sival_ptr = this;
	};
	~SockoutSingle()
	{
		delete m_pReq;
	};
	static void SockoutReadCompletionHandler( sigval_t sigval );
};
class StdinSingle
{
public:

	StdinSingle(char *pKey,int nSock,sem_t &bStop)
		:m_bStop(bStop)
	{
		memcpy(m_strKey,pKey,8);
		m_strKey[8]=0;
		this->m_nSock=nSock;
		bzero(&strStdinBuffer, BUFFERSIZE);
		this->m_pReq = new aiocb;
		bzero( (char *)m_pReq, sizeof(struct aiocb) );
		m_pReq->aio_fildes = 0;
		m_pReq->aio_buf = strStdinBuffer;
		m_pReq->aio_nbytes = BUFFERSIZE;
		m_pReq->aio_offset = 0;
		m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._function =	StdinReadCompletionHandler;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;
		m_pReq->aio_sigevent.sigev_value.sival_ptr = this;
	};
	~StdinSingle()
	{
		delete m_pReq;
		bzero(&strStdinBuffer, BUFFERSIZE);
	};
	char m_strKey[9];
	int m_nSock;
	aiocb *m_pReq;
	sem_t &m_bStop;
//	char strStdinBuffer[BUFFERSIZE];

	static void StdinReadCompletionHandler( sigval_t sigval )
	{
		StdinSingle* pThis = (StdinSingle*)sigval.sival_ptr;
		  /* Did the request complete? */
		  
		if (aio_error( pThis->m_pReq ) == 0) 
		{
			/* Request completed successfully, get the return status */
			int nSize = aio_return( pThis->m_pReq );
			CDesOperate cDes;
			int nLen = BUFFERSIZE;
			cDes.Encry(strStdinBuffer,BUFFERSIZE,strEncryBuffer,nLen,pThis->m_strKey,8);


			SockoutSingle *pSockoutSingle = new SockoutSingle(pThis->m_nSock,pThis->m_strKey,pThis->m_bStop); 
			aio_write(pSockoutSingle->m_pReq);
			if(0==memcmp("quit",strStdinBuffer,4))
			{
				printf("Quit!\n");
				sem_post(&pThis->m_bStop); 
				exit(0);
			}
		  
		}
		delete pThis;
		return;
	};


};
void SockoutSingle::SockoutReadCompletionHandler( sigval_t sigval )
{
	SockoutSingle* pThis = (SockoutSingle*)sigval.sival_ptr;
		
	if (aio_error( pThis->m_pReq ) == 0) 
	{
		/* Request completed successfully, get the return status */
		int nSize = aio_return( pThis->m_pReq );
		if(nSize != BUFFERSIZE)
		{
			perror("Error Send!\n");
		}
		else
		{
			StdinSingle *pStdin = new StdinSingle(pThis->m_strKey,pThis->m_nSock,pThis->m_bStop);
			aio_read(pStdin->m_pReq);
		}
	  
	}
	delete pThis;
	return;
}



class StdoutSingle
{
public:
	char m_strKey[9];
	int m_nSock;
	aiocb *m_pReq;
	sem_t &m_bStop;
	StdoutSingle(int nSock,char *pKey,sem_t &bStop)
		:m_bStop(bStop)
	{
		memcpy(m_strKey,pKey,8);
		m_strKey[8]=0;
		this->m_nSock=nSock;
		this->m_pReq = new aiocb;
		bzero( (char *)m_pReq, sizeof(struct aiocb) );
		m_pReq->aio_fildes = 1;
		m_pReq->aio_buf = strStdoutBuffer;
		m_pReq->aio_nbytes = strlen(strStdoutBuffer);
		m_pReq->aio_offset = 0;
		m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._function = StdoutReadCompletionHandler;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;
		m_pReq->aio_sigevent.sigev_value.sival_ptr = this;
	};
	~StdoutSingle()
	{
		delete m_pReq;
	};
	static void StdoutReadCompletionHandler( sigval_t sigval );
};
class SockinSingle
{
	public:

	SockinSingle(char *pKey,int nSock,sem_t &bStop)
		:m_bStop(bStop)
	{
		memcpy(m_strKey,pKey,8);
		m_strKey[8]=0;
		this->m_nSock=nSock;
		bzero(&strSocketBuffer, BUFFERSIZE);
		this->m_pReq = new aiocb;
		bzero( (char *)m_pReq, sizeof(struct aiocb) );
		m_pReq->aio_fildes = nSock;
		m_pReq->aio_buf = strSocketBuffer;
		m_pReq->aio_nbytes = BUFFERSIZE;
		m_pReq->aio_offset = 0;
		m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._function =	SockinSingleReadCompletionHandler;
		m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;
		m_pReq->aio_sigevent.sigev_value.sival_ptr = this;
	};
	~SockinSingle()
	{
		delete m_pReq;
		bzero(&strSocketBuffer, BUFFERSIZE);
	};
	char m_strKey[9];
	int m_nSock;
	aiocb *m_pReq;
	sem_t &m_bStop;
//	char strStdinBuffer[BUFFERSIZE];

	static void SockinSingleReadCompletionHandler( sigval_t sigval )
	{
		SockinSingle* pThis = (SockinSingle*)sigval.sival_ptr;
		if (aio_error( pThis->m_pReq ) == 0) 
		{
			int nSize = aio_return( pThis->m_pReq );
			if(nSize != BUFFERSIZE)
			{
				printf("receive error\n");
				sem_post(&pThis->m_bStop); 
				return;
			}
			 
			CDesOperate cDes;
			int nLen = BUFFERSIZE;
			cDes.Decry(strSocketBuffer,BUFFERSIZE,strDecryBuffer,nLen,pThis->m_strKey,8);
		/*	printf("Receive message: %s\n",strDecryBuffer);*/
			sprintf(strStdoutBuffer,"Receive message: %s\n",strDecryBuffer);
			
			StdoutSingle *pStdoutSingle = new StdoutSingle(pThis->m_nSock,pThis->m_strKey,pThis->m_bStop); 
			aio_write(pStdoutSingle->m_pReq);
			if(0==memcmp("quit",strDecryBuffer,4))
			{
				printf("Quit!\n");
				sem_post(&pThis->m_bStop); 
				exit(0);
			}
		  
		}
		delete pThis;
		return;
	};


};

void StdoutSingle::StdoutReadCompletionHandler( sigval_t sigval )
{
	StdoutSingle* pThis = (StdoutSingle*)sigval.sival_ptr;
		
	if (aio_error( pThis->m_pReq ) == 0) 
	{
		/* Request completed successfully, get the return status */
		int nSize = aio_return( pThis->m_pReq );
		if(nSize != pThis->m_pReq->aio_nbytes)
		{
			perror("Error Send!\n");
		}
		else
		{
			SockinSingle *pSockin = new SockinSingle(pThis->m_strKey,pThis->m_nSock,pThis->m_bStop);
			aio_read(pSockin->m_pReq);
		}
	  
	}
	delete pThis;
	return;
}
void SecretChat(int nSock,char *pRemoteName, char *pKey)
{
	pid_t nPid;
	nPid = fork();
	sem_t bStop;
	sem_init(&bStop,0,0); 
	if(nPid != 0)
	{
		
		SockinSingle *pSockin = new SockinSingle(pKey,nSock,bStop);
		aio_read(pSockin->m_pReq);
		sem_wait(&bStop); 

	}
	else
	{
		StdinSingle *pStdin = new StdinSingle(pKey,nSock,bStop);
		aio_read(pStdin->m_pReq);
		sem_wait(&bStop); 

	}		
	sem_destroy(&bStop);
}

#endif

void Chat(int nSock,char *pRemoteName)
{
	pid_t nPid;
	nPid = fork();
	if(nPid != 0)
	{
		while(1)
		{
			bzero(&strSocketBuffer, BUFFERSIZE);
			int nLength = 0;
			nLength = TotalRecv(nSock, strSocketBuffer,BUFFERSIZE,0);
			if(nLength !=BUFFERSIZE)
			{
				break;
			}
			else
			{
				strSocketBuffer[BUFFERSIZE-1]=0;
				if(strSocketBuffer[0]!=0&&strSocketBuffer[0]!='\n')
				{
					printf("Receive message form <%s>: %s\n", pRemoteName,strSocketBuffer);
					if(0==memcmp("quit",strSocketBuffer,4))
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
			
			if(send(nSock, strStdinBuffer, BUFFERSIZE,0)!=BUFFERSIZE)
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
	PublicKey cRsaPublicKey;
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
			printf("Connect Success!  \n");
			GerenateDesKey(strDesKey);
			printf("Create DES key success\n");
			if(sizeof(cRsaPublicKey)==TotalRecv(nConnectSocket,(char *)&cRsaPublicKey,sizeof(cRsaPublicKey),0))
			{
				printf("Successful get the RSA public Key\n");	
			}
			else
			{
				perror("Get RSA public key ");
				exit(0);
			}
			UINT64 nEncryptDesKey[DESKEYLENGTH/2];
			unsigned short *pDesKey = (unsigned short *)strDesKey;
			for(int i = 0;i<DESKEYLENGTH/2;i++)
			{
				nEncryptDesKey[i] = CRSASection::Encry(pDesKey[i],cRsaPublicKey);
			}
			if(sizeof(UINT64 )*DESKEYLENGTH/2!=send(nConnectSocket,(char *)nEncryptDesKey,sizeof(UINT64 )*DESKEYLENGTH/2,0))
			{
				perror("Send DES key Error");
				exit(0);
			}
			else
			{
				printf("Successful send the encrypted DES Key\n");	
			}
			printf("Begin to chat...\n");
			SecretChat(nConnectSocket,strIpAddr,strDesKey);
			printf("Client Finished. \n");
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
			CRSASection cRsaSection;
			cRsaPublicKey = cRsaSection.GetPublicKey();
			if(send(nAcceptSocket, (char *)(&cRsaPublicKey), sizeof(cRsaPublicKey),0)!=sizeof(cRsaPublicKey))
			{
				perror("send");
				exit(0);
			}
			else
			{
				printf("successful send the RSA public key. \n");
			}
			UINT64 nEncryptDesKey[DESKEYLENGTH/2];
			if(DESKEYLENGTH/2*sizeof(UINT64) != TotalRecv(nAcceptSocket, (char *)nEncryptDesKey,DESKEYLENGTH/2*sizeof(UINT64),0))
			{
				perror("TotalRecv DES key error");
				exit(0);
			}
			else
			{
				printf("successful get the DES key. \n");
				unsigned short * pDesKey = (unsigned short *)strDesKey;
				for(int i = 0;i<DESKEYLENGTH/2;i++)
				{
					pDesKey[i] = cRsaSection.Decry(nEncryptDesKey[i]);
				}
			}
			printf("Begin to chat...\n");
			SecretChat(nAcceptSocket,inet_ntoa(sRemoteAddr.sin_addr),strDesKey);
			close(nAcceptSocket);
			
		}
	}
	return 0;
}
