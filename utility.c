#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void print_message(const char *fmt, ...){
    fprintf(stderr, "%s\n", fmt);
    fflush(stdout);
}

void handle_error(const char *fmt, ...){
    print_message(fmt);
    exit(EXIT_FAILURE);
}

int max(int v1, int v2){
    return v1 > v2 ? v1 : v2;
}

int compute(char *input){  
    char *operators = "+-/*";
    char operator;
    for(int i = 0; i < strlen(input); i++){
        if(strchr(operators, input[i])){
            operator = input[i];
            break;
        }
    }
    int num1 = 0, num2 = 0;
    char *ptr = strtok(input, "*+-/");
    num1 = atoi(ptr);
    ptr = strtok(NULL, "*+-/");
    num2 = atoi(ptr);
    switch (operator){
        case '+':
            return num1+num2;
        case '-':
            return num1-num2;
        case '*':
            return num1*num2;
        case '/':
            return num1/num2;
        default:
            return 0;
    }
}
