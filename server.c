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

// Standard http port
#define SERVER_PORT 3000
// Buffer size
#define MAXLINE 4096
// Sockaddr
#define SA struct sockaddr
// Max connections
#define MAX_CONNECTIONS 1

void err_n_die(const char *fmt, ...){
    fprintf(stderr, "%s\n", fmt);
    fflush(stdout);
    exit(1);
}

int convert_to_int(char *num){
    char *local_num = num;
    int res = 0;
    while(local_num){
        res += *local_num -'0';
        res *= 10;
        local_num++;
    }
    return res;
}

void* main_thread(void *arg) {
    int connfd = *((int *) arg);
    char receiveline[MAXLINE+1];

    while(1){
        if(read(connfd, receiveline, MAXLINE) < 0)
            err_n_die("error reading from socket!");
        
        fprintf(stdout, "Message received!: %s", receiveline);

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
}

void handle_request(int connfd){
    char receiveline[MAXLINE+1];

    // while(1){
    if(read(connfd, receiveline, MAXLINE) < 0)
        err_n_die("error reading from socket!");
    
    fprintf(stdout, "Message received!: %s", receiveline);
    receiveline[strlen(receiveline)] = '\0';

    int num1 = convert_to_int(strtok(receiveline, "+-*/"));
    int num2 = convert_to_int(strtok(NULL, "+-*/"));
    int result;
    
    if(strchr(receiveline, '+')){
        result = num1 + num2;
    }else if(strchr(receiveline, '-')){
        result = num1 - num2;
    }else if(strchr(receiveline, '*')){
        result = num1 * num2;
    }else{
        result = num1 / num2;
    }

    fprintf(stdout, "%d", result);
    fflush(stdout);

    char *message = receiveline;
    if(write(connfd, message, strlen(message)) < 0)
        err_n_die("error writing to socket!");
    // }
}

int main(int argc, char **argv){
    struct sockaddr_in servaddr;
    char sendline[MAXLINE+1];

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    while(1){
        struct sockaddr_in addr;
        socklen_t addr_len;
        int listenfd, connfd;

        fprintf(stdout, "setting up socket\n");
        fflush(stdout);
        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            err_n_die("socket error!");

        fprintf(stdout, "binding the socket\n");
        fflush(stdout);
        if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
            if(errno == EADDRINUSE || errno == EADDRINUSE){
                fprintf(stdout, "port or address already in use, trying to reconnect in 10 seconds\n");
                fflush(stdout);
                sleep(10);
                continue;
            }
            err_n_die("bind error.");
        }
    

        fprintf(stdout, "setting up the listener\n");
        fflush(stdout);
        if(listen(listenfd, 0) < 0)
            err_n_die("listening failed");

        fprintf(stdout, "socket established\n");
        fflush(stdout);
        fprintf(stdout, "waiting for connections\n");
        fflush(stdout);

        connfd = accept(listenfd, (SA *) NULL, NULL);
        fprintf(stdout, "client connected.\n");
        fflush(stdout);
        close(listenfd);

        handle_request(connfd);
        close(connfd);
        fprintf(stdout, "client disconnected.\n");
        fflush(stdout);
        
        sleep(5);
    }

    exit(0);
}