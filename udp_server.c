#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUF_SIZE 50

int main(int argc, char *argv[])
{
  int s;
  in_port_t myport;
  socklen_t sktlen;
  struct sockaddr_in myskt;
  struct sockaddr_in skt;
  char recv_buf[100];
  int count;
  fd_set readfds, fds;
  char buf[BUF_SIZE];
  char *myip, *oppip;

  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }
  myport = atoi(argv[1]);
  myip = argv[2];
  memset(&myskt, 0, sizeof myskt);
  myskt.sin_family = AF_INET;
  myskt.sin_port = htons(myport);
  myskt.sin_addr.s_addr = inet_addr(myip);

  oppip = argv[3];
  memset(&skt, 0, sizeof skt);
  skt.sin_family = AF_INET;
  skt.sin_port = htons(myport);
  skt.sin_addr.s_addr = inet_addr(oppip);


  if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
    perror("bind");
    exit(1);
  }

  FD_ZERO(&readfds);
  FD_SET(s, &readfds);
  FD_SET(0, &readfds);
  
  for (;;) {
	  memcpy(&fds, &readfds, sizeof(fd_set));
	  select(s + 1, &fds, NULL, NULL, NULL);

	  if (FD_ISSET(s, &fds)) {
		  sktlen = sizeof skt;
                  memset(recv_buf, 0, sizeof(recv_buf));
		  if (recvfrom(s, recv_buf, sizeof recv_buf, 0, (struct sockaddr *)&skt, &sktlen) < 0) {
			  perror("recvfrom");
			  exit(1);
		  }
		  printf("received message: %s\n", recv_buf);
/*
		  if ((count = sendto(s, recv_buf, sizeof recv_buf, 0, (struct sockaddr *) &skt, sizeof skt)) < 0) {
			  perror("sendto");
			  exit(1);
		  }*/
	  }
	  if (FD_ISSET(0, &fds)) {
                  memset(buf, 0, sizeof(buf));
		  read(0, buf, BUF_SIZE);
		  if (sendto(s, buf, sizeof buf, 0, (struct sockaddr *)&skt, sizeof skt) < 0) {
	                  perror("sendto");
		  }
	  }
	  }
  	
    /* sktlen = sizeof skt;
    if ((count = recvfrom(s, recv_buf, sizeof recv_buf, 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
      perror("recvfrom");
      exit(1);
    }*/
/*)
    printf("received message: %s\n", recv_buf);
    if ((count = sendto(s, recv_buf, sizeof recv_buf, 0, (struct sockaddr *) &skt, sizeof skt)) < 0) {
      perror("sendto");
      exit(1);
    }
  }*/
  close(s);
  return 0;
}
