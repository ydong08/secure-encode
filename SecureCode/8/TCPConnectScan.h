#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "DataStructure.h"

using namespace std;

//===========Global Variable===================================
int TCPConThrdNum;
//locker
pthread_mutex_t TCPConPrintlocker=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t TCPConScanlocker=PTHREAD_MUTEX_INITIALIZER;

