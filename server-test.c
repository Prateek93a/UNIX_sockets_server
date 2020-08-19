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

// Standard http port
#define SERVER_PORT 5555
// Buffer size
#define MAXLINE 4096
// Sockaddr
#define SA struct sockaddr
// Max connections
#define MAX_CONNECTIONS 1

void err_n_die(const char *fmt, ...){
    fprintf(stderr, "%s", fmt);
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
    // }
}

int main(int argc, char **argv){
    // if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
    //     err_n_die("bind error.");
    
    // if(listen(listenfd, 0) < 0)
    //     err_n_die("listening failed.");
    
    while(1){
        struct sockaddr_in addr;
        socklen_t addr_len;
        int listenfd, connfd;
        struct sockaddr_in servaddr;
        char sendline[MAXLINE+1];
    
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            err_n_die("socket error!");

        if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
            err_n_die("bind error.");
    
        if(listen(listenfd, 0) < 0)
            err_n_die("listening failed.");

        printf("waiting for connections\n");
        connfd = accept(listenfd, (SA *) NULL, NULL);
        close(listenfd);

        handle_request(connfd);
        close(connfd);

        // if(fork() == 0){
        //     handle_request(connfd);
        // }

        // pthread_t thread;
        // pthread_create(&thread, NULL, main_thread, &connfd);
        // pthread_join(thread, NULL);
    }

    exit(0);
}