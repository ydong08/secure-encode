#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include "DataStructure.h"

using namespace std;

//===========Global Variable=======================================
//locker
pthread_mutex_t UDPPrintlocker=PTHREAD_MUTEX_INITIALIZER;

//============Function=============================================
extern unsigned short in_cksum(unsigned short *ptr, int nbytes);


 
