#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
// #define MAX_INPUT_SIZE 256
// int main(int argc, char *argv[])
// {
//  int sockfd, portnum, n;
//  struct sockaddr_in server_addr;
//  char inputbuf[MAX_INPUT_SIZE];
//  if (argc < 3) {
//  fprintf(stderr,"usage %s <server-ip-addr> <server-port>\n",
// argv[0]);
//  exit(0);
//  }
//  portnum = atoi(argv[2]);
//  /* Create client socket */
//  sockfd = socket(AF_INET, SOCK_STREAM, 0);
//  if (sockfd < 0)
//  {
// fprintf(stderr, "ERROR opening socket\n");
// exit(1);
//  }
//  /* Fill in server address */
//  bzero((char *) &server_addr, sizeof(server_addr));
//  server_addr.sin_family = AF_INET;
//  if(!inet_aton(argv[1], &server_addr.sin_addr))
//  {
// fprintf(stderr, "ERROR invalid server IP address\n");
// exit(1);
//  }
//  server_addr.sin_port = htons(portnum);
//   /* Connect to server */
//  if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))
// < 0)
//  {
// fprintf(stderr, "ERROR connecting\n");
// exit(1);
//  }
//  printf("Connected to server\n");

//  do
//  {
// /* Ask user for message to send to server */
// printf("Please enter the message to the server: ");
// bzero(inputbuf,MAX_INPUT_SIZE);
// fgets(inputbuf,MAX_INPUT_SIZE-1,stdin);
// /* Write to server */
// n = write(sockfd,inputbuf,strlen(inputbuf));
// if (n < 0)
//  {
//  fprintf(stderr, "ERROR writing to socket\n");
//  exit(1);
//  }
// /* Read reply */
// bzero(inputbuf,MAX_INPUT_SIZE);
// n = read(sockfd,inputbuf,(MAX_INPUT_SIZE-1));
// if (n < 0)
//  {
//  fprintf(stderr, "ERROR reading from socket\n");
//  exit(1);
//  }
// printf("Server replied: %s\n",inputbuf);
//  } while(1);
//  return 0;
// }

// Standard http port
#define SERVER_PORT 80
// Buffer size
#define MAXLINE 4096
// Sockaddr
#define SA struct sockaddr

void err_n_die(const char *fmt, ...){
    fprintf(stderr, "%s", fmt);
    exit(0);
}

int main(int argc, char **argv){
    int sockfd, n;
    int sendBytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char receiveline[MAXLINE];

    if(argc != 2)
        err_n_die("usage: %s <server address>", argv[0]);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error while creating a socket!");

    
}