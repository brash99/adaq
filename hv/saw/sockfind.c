#ifdef VXWORKS
#include <sockLib.h> 
#include <inetLib.h> 
#include <stdioLib.h>
#include <usrLib.h>
#include <ioLib.h>
#include <taskLib.h>
#include <semLib.h>
#include <vxWorks.h>
#include <stdio.h>
#include <signal.h>
#endif

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int sockfind (int sockfd)
{
  
  struct sockaddr_in serverAddr; 
  int slen = sizeof(serverAddr)+100;

  int sFd;                       /* socket file descriptor */
  int port;
  int optval;
  int optlen=sizeof(optval);

  for(sFd=1;sFd<32;sFd++) {
    /*printf("Checking FD %d\n",sFd);*/
    if(getsockopt(sFd,SOL_SOCKET,SO_TYPE, (void *) &optval, &optlen) < 0) {
      continue;
    }
    /*printf("Type = %d\n",optval);*/
    if(optval>0) {
      if(getsockname(sFd, (struct sockaddr *)&serverAddr, &slen) < 0) {
	printf("Socket %d not found\n",sFd);
      } else {
	port = ntohs(serverAddr.sin_port);
	printf("FD %d (%d) %d\n",sFd,optval,port);
	/* printf("Socket %d has port %d\n", sFd, port);*/
	if(port==2014) {
	  close(sFd);
	}
      }
    }
  }
  return 0;
}
