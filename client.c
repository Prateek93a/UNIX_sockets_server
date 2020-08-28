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
#include "utility.h"

#define SERVER_PORT 3000
#define MAXLINE 4096
#define SA struct sockaddr
#define TRUE 1

int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE+1];
    char receiveline[MAXLINE+1];

    if(argc != 2)
        handle_error("usage: %s <server address>", argv[0]);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        handle_error("error while creating a socket!");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        handle_error("inet_pton error for %s", argv[1]);

    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
        if(errno == ECONNREFUSED)
            handle_error("Connection busy!");
        else
            handle_error("connect failed!");
    }

    print_message("server connected!\n");

    while(TRUE){
        bzero(sendline, MAXLINE);
        bzero(receiveline, MAXLINE);

        print_message("Enter the request: ");
        scanf("%s", sendline);

        if(strcmp(sendline, "exit") == 0){
            print_message("Closed the client socket.\n");
            close(sockfd);
            break;
        }

        if(write(sockfd, sendline, strlen(sendline)) < 0)
            handle_error("error writing to socket!");

        if(read(sockfd, receiveline, MAXLINE) < 0)
            handle_error("error reading from socket!");

        printf("Server Replied: %s\n", receiveline);
        fflush(stdin);
    }

    exit(EXIT_SUCCESS);
}