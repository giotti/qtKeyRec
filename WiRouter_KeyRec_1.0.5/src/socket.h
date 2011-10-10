
#ifndef SOCKET_H
#define SOCKET_H

#ifdef WIN32
    #include <winsock.h>
    #define close closesocket
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <errno.h>
	#include <netdb.h>
#endif


unsigned char *socket_receive(int sock, int tout);
int socket_connect(unsigned char *server, int port);
int vspr(char **buff, char *fmt, ...);

#endif