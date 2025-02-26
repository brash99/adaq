#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>

#define BUFLEN 2000
int main(int argc, char *argv[]) {
  int sock, valread;
  struct sockaddr_in server;
  struct timeval tv;
  unsigned char buf[BUFLEN+1];
  int i;
  
  int port=2012;
  char *address="129.57.36.152";

  signal(SIGPIPE, SIG_IGN);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("Could not create socket. Error");
    return 1;
  }
  server.sin_addr.s_addr = inet_addr(address);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
  //  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval));
  
  printf("Connected...\n");

  //  valread = read(sock, buf, BUFLEN);
  //  if(valread < 0) {
  //    printf("Error reading from socket %d\n",valread);
  //    return 1;
  //  }
  while(1) {
    strcpy(buf,"GS");
    printf("Sending %s\n",buf);
    i = send(sock, buf, strlen(buf)+1, 0);
    printf("Sent GS %d\n",i);
    valread = recv(sock, buf, BUFLEN, 0);
    //    valread = read(sock, buf, BUFLEN);
    printf("Read %d characters\n",valread);
    if(valread < 0 || i < 0) {
      printf("Error reading from socket %d\n",valread);
      printf("%s\n",buf);
      close(sock);
      printf("Trying to reconnect\n");
      server.sin_addr.s_addr = inet_addr(address);
      server.sin_family = AF_INET;
      server.sin_port = htons(port);
      sock = socket(AF_INET, SOCK_STREAM, 0);
      i = connect(sock , (struct sockaddr *)&server , sizeof(server));      
      setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
      printf("Connected %d\n",i);
    } else {
      int j;
      for(j=0;j<valread;j++) {
	if(buf[j] == '\0') buf[j] = '\n';
      }
      buf[valread+1] = '\0';
      printf("%s\n",buf);
    //    for(i=0;i<strlen(buf);i++) {
    //      printf("%d: %x\n",i,buf[i]);
      //    }
    }
    sleep(5);
  }
  return 0;
}
  
