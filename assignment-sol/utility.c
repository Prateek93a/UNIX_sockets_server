#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void print_message(const char *fmt, ...){
    fprintf(stderr, "%s\n", fmt);
    fflush(stdout);
}

void err_n_die(const char *fmt, ...){
    print_message(fmt);
    exit(EXIT_FAILURE);
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
