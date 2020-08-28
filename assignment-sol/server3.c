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
#include <sys/time.h>
#include <sys/select.h>
#include "utility.h"

#define SERVER_PORT 3000
#define MAXLINE 4096
#define SA struct sockaddr
#define MAX_CONNECTIONS 10
#define TRUE 1

void handle_request(int connfd, fd_set* current_sockets){
    char receiveline[MAXLINE+1];
    char sendline[MAXLINE+1];
    int rsig; 
    if((rsig = read(connfd, receiveline, MAXLINE)) <= 0){
        if(rsig == 0){
            close(connfd);
            FD_CLR(connfd, current_sockets);
            print_message("client disconnected.\n");
            return;
        }
        err_n_die("error reading from socket!");
    }
    print_message("message received\n");
    int result = compute(receiveline);
    sprintf(sendline, "%d", result);
    if(write(connfd, sendline, strlen(sendline)) < 0)
        err_n_die("error writing to socket!");
    print_message("response sent\n");
}


int main(int argc, char **argv){
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int listenfd, connfd;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error!");

    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_n_die("bind error.");

    if(listen(listenfd, MAX_CONNECTIONS) < 0)
        err_n_die("listening failed");

    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(listenfd, &current_sockets);

    print_message("waiting for connection\n");

    while(TRUE){
        ready_sockets = current_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
            err_n_die("select error.\n");
        
        for(int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &ready_sockets)){
                if(i == listenfd){
                    connfd = accept(listenfd, (SA *) NULL, NULL);
                    FD_SET(connfd, &current_sockets);
                    print_message("client connected.\n");
                }else{
                    handle_request(i, &current_sockets);
                }
            }
        }   
    }

    exit(EXIT_SUCCESS);
}
