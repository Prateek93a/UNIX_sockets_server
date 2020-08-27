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
#define MAX_CONNECTIONS 1
#define TRUE 1

void handle_request(int connfd){
    char receiveline[MAXLINE+1];
    char sendline[MAXLINE+1];

    while(TRUE){
        int rsig; 
        if((rsig = read(connfd, receiveline, MAXLINE)) <= 0){
            if(rsig == 0) break;
            err_n_die("error reading from socket!");
        }
        int result = compute(receiveline);
        sprintf(sendline, "%d", result);
        if(write(connfd, sendline, strlen(sendline)) < 0)
            err_n_die("error writing to socket!");
    }
    close(connfd);
    print_message("client disconnected.\n");
}


int main(int argc, char **argv){
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    while(TRUE){
        int listenfd, connfd;

        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            err_n_die("socket error!");

        if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
            if(errno == EADDRINUSE || errno == EADDRINUSE){
                print_message("port or address already in use, trying to reconnect in 10 seconds\n");
                sleep(10);
                continue;
            }
            err_n_die("bind error.");
        }

        if(listen(listenfd, MAX_CONNECTIONS) < 0)
            err_n_die("listening failed");

        print_message("socket established\nwaiting for connection");

        connfd = accept(listenfd, (SA *) NULL, NULL);
        print_message("client connected.\n");
        close(listenfd);
        handle_request(connfd);   
        print_message("setting up connection again.\n");   
    }

    exit(EXIT_SUCCESS);
}