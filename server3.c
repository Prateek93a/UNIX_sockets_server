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
#include <sys/select.h>

// Standard http port
#define SERVER_PORT 3000
// Buffer size
#define MAXLINE 4096
// Sockaddr
#define SA struct sockaddr
// Max connections
#define MAX_CONNECTIONS 100

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

int max(int v1, int v2){
    return v1 > v2 ? v1 : v2;
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
    int clients[MAX_CONNECTIONS];
    int num_connections = 0;
    int max_client_fd = -1;
    int listenfd;
    fd_set rfds;
    struct timeval tv;
    int retval;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // if((listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error!");

    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_n_die("bind error.");
    
    if(listen(listenfd, 10) < 0)
        err_n_die("listening failed");

    fprintf(stdout, "waiting for connections\n");
    fflush(stdout);
    
    while(1){
        int connfd;
        int i = 0;

        FD_ZERO(&rfds);
        FD_SET(listenfd, &rfds);
        
        if(select(listenfd+1, &rfds, NULL, NULL, &tv) > 0){
            connfd = accept(listenfd, (SA *) NULL, NULL);
            fprintf(stdout, "client connected.\n");
            fflush(stdout);
            max_client_fd = max(max_client_fd, connfd);
            clients[num_connections++] = connfd;
        }
  
        
        FD_ZERO(&rfds);
        for(i = 0; i < num_connections; i++)
            FD_SET(clients[i], &rfds);

        if(select(max_client_fd+1, &rfds, NULL, NULL, &tv) > 0){
            for(i = 0; i < num_connections; i++){
                if(FD_ISSET(0, &rfds)){
                    handle_request(clients[i]);
                }
            } 
        }

        // if (retval == -1)
        //     err_n_die('error regarding select system call.');
        // else if (retval)
        //     printf("Data is available now.\n");
        //     /* FD_ISSET(0, &rfds) will be true. */

        // handle_request(connfd);
        // close(connfd);
        // fprintf(stdout, "client disconnected.\n");
        // fflush(stdout);
    }

    exit(0);
}