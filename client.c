#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


// Standard http port
#define SERVER_PORT 5555
// Buffer size
#define MAXLINE 4096
// Sockaddr
#define SA struct sockaddr

void err_n_die(const char *fmt, ...){
    fprintf(stderr, "%s", fmt);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    int sockfd, n;
    int sendBytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE+1];
    char receiveline[MAXLINE+1];

    if(argc != 2)
        err_n_die("usage: %s <server address>", argv[0]);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("error while creating a socket!");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_n_die("inet_pton error for %s", argv[1]);

    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
        if(errno == ECONNREFUSED)
            err_n_die("Connection busy!");
        else
            err_n_die("connect failed!");
    }

    fprintf(stdout, "server connected!\n");

    while(1){
        bzero(sendline, MAXLINE);
        bzero(receiveline, MAXLINE);

        printf("Enter the request: ");
        fgets(sendline, MAXLINE, stdin);

        if(strcmp(sendline, "exit") == 0){
            printf("Closed the client socket.\n");
            close(sockfd);
            break;
        }

        if(write(sockfd, sendline, strlen(sendline)) < 0)
            err_n_die("error writing to socket!");

        if(read(sockfd, receiveline, MAXLINE) < 0)
            err_n_die("error reading from socket!");

        printf("Server Replied!!! - %s\n", receiveline);
    }

    exit(EXIT_SUCCESS);
}