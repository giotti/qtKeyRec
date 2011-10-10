
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "socket.h"
#include "wirouterkeyrec.h"


int socket_connect(unsigned char *server, int port) {

  int fd;
  struct sockaddr_in sock;
  struct hostent *host;

#ifdef WIN32	
  WSADATA wsadata;
  if(WSAStartup(MAKEWORD(1,0), &wsadata)) return -1;
#endif

  memset(&sock, 0, sizeof(sock));

  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;

  sock.sin_family = AF_INET;
  sock.sin_port = htons(port);

  if(!(host=gethostbyname((char *)server))) return -1;

  sock.sin_addr = *((struct in_addr *)host->h_addr);

  if(connect(fd, (struct sockaddr *) &sock, sizeof(sock)) < 0) return -1;

  return fd;
   
}



unsigned char *socket_receive(int sock, int tout) {

  int ret,
      byte,
      oldpkglen = 0,
      pkglen    = 0;

  unsigned char *buffer = NULL, 
		tmp[128];

  struct timeval timeout;
  fd_set input;

  if(sock < 0) return NULL;

  while (1) {

    FD_ZERO(&input);
    FD_SET(sock, &input);

    if(tout > 0) {
      timeout.tv_sec  = tout;
      timeout.tv_usec = 0;
      ret = select(sock + 1, &input, NULL, NULL, &timeout);
    }
    else
      ret = select(sock + 1, &input, NULL, NULL, NULL);

    if(!ret)    break;
    if(ret < 0) return NULL;

    byte = recv(sock, (char *)tmp, sizeof(tmp), 0);

    if(byte < 0) return NULL;
    if(!byte)    break;

    oldpkglen = pkglen;
    pkglen    += byte;

    buffer = (unsigned char *) realloc(buffer, pkglen+1);

    if(!buffer) return NULL;
      
    memcpy(buffer+oldpkglen, tmp, byte);

  }

  if(buffer) buffer[pkglen] = 0x00;

  return buffer;
   
}


