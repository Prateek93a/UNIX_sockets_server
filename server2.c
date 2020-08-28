#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "utility.h"

#define SERVER_PORT 3000
#define MAXLINE 4096
#define SA struct sockaddr
#define MAX_CONNECTIONS 10
#define TRUE 1

void handle_request(int connfd){
    char receiveline[MAXLINE+1];
    char sendline[MAXLINE+1];

    while(TRUE){
        int rsig; 
        if((rsig = read(connfd, receiveline, MAXLINE)) <= 0){
            if(rsig == 0) break;
            handle_error("error reading from socket!");
        }
        print_message("message received\n");
        int result = compute(receiveline);
        sprintf(sendline, "%d", result);
        if(write(connfd, sendline, strlen(sendline)) < 0)
            handle_error("error writing to socket!");
        print_message("response sent\n");
    }
    close(connfd);
    print_message("client disconnected.\n");
}

int main(int argc, char **argv){
    struct sockaddr_in servaddr;
    int listenfd;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        handle_error("socket error!");

    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        handle_error("bind error.");

    if(listen(listenfd, MAX_CONNECTIONS) < 0)
        handle_error("listening failed");

    while(TRUE){
        int connfd;
        print_message("waiting for connections\n");

        connfd = accept(listenfd, (SA *) NULL, NULL);
        print_message("client connected.\n");

        if(!fork()){
            handle_request(connfd);
            exit(EXIT_SUCCESS);
        }

        close(connfd);
    }

    exit(EXIT_SUCCESS);
}