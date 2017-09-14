# Generated automatically from Makefile.in by configure.
CC=g++
CFLAGS=-g -I/usr/local/openssl/include/  -Wall
LD=-L/usr/local/openssl/lib  -lssl -lcrypto -ldl -lpthread
DISTDIR=Test

DIST=common.cpp \
	common.h \
	HttpProtocol.cpp \
	HttpProtocol.h \
	MyWebServer.cpp \
  
MyWebServer: common.o MyWebServer.o HttpProtocol.o
	$(CC) common.o MyWebServer.o HttpProtocol.o -o MyWebServer $(LD)

clean:	
	rm *.o MyWebServer
dist:
	rm -rf ${DISTDIR}; mkdir ${DISTDIR}
	cp ${DIST} ${DISTDIR}
	rm -f ${DISTFILE}
	tar cf - ${DISTDIR} | gzip > ${DISTFILE}
