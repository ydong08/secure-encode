#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#define __FAVOR_BSD            // use bsd'ish tcp header 
#include "DataStructure.h"

//===========Global Variable===================================
int TCPFinThrdNum;

//locker
pthread_mutex_t TCPFinPrintlocker=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t TCPFinScanlocker=PTHREAD_MUTEX_INITIALIZER;
//============Function===========================================
extern unsigned short in_cksum(unsigned short *ptr, int nbytes);
