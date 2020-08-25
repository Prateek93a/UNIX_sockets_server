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

    while(TRUE){
        int rsig; 
        if((rsig = read(connfd, receiveline, MAXLINE)) <= 0){
            if(rsig == 0) break;
            err_n_die("error reading from socket!");
        }
        print_message("Message received!: %s", receiveline);
        // receiveline[strlen(receiveline)] = '\0';

        // int num1 = convert_to_int(strtok(receiveline, "+-*/"));
        // int num2 = convert_to_int(strtok(NULL, "+-*/"));
        // int result;
        
        // if(strchr(receiveline, '+')){
        //     result = num1 + num2;
        // }else if(strchr(receiveline, '-')){
        //     result = num1 - num2;
        // }else if(strchr(receiveline, '*')){
        //     result = num1 * num2;
        // }else{
        //     result = num1 / num2;
        // }

        // fprintf(stdout, "%d", result);
        // fflush(stdout);

        char *message = receiveline;
        if(write(connfd, message, strlen(message)) < 0)
            err_n_die("error writing to socket!");
    }
    close(connfd);
    print_message("client disconnected.\n");
}


int main(int argc, char **argv){
    struct sockaddr_in servaddr;
    char sendline[MAXLINE+1];

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    while(TRUE){
        struct sockaddr_in addr;
        socklen_t addr_len;
        int listenfd, connfd;

        fprintf(stdout, "setting up socket\n");
        fflush(stdout);

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