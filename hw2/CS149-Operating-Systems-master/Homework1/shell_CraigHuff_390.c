//
//  main.c
//  Homework1
//
//  Created by Craig on 9/25/17.
//  Copyright © 2017 Craig. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define MAXLINE 80

/* The maximum length command */
int main(void)
{   char *args[MAXLINE/2 + 1];
    char str[MAXLINE/2 + 1];
    char exit2[5] = "exit\n";
   
    pid_t pid;
    int status;
    /* command line with max 40 arguments */
    int should_run = 1; /* flag to determine when to exit program */
    printf("CS149 Shell from Craig Huff\n");
    /* replace w/ name */
    while (should_run == 1) {
        int counter = 0;
        printf("Craig-390> "); /* prompt- replace FirstName and L3SID */
        scanf("%[^\n]%*c", str);
        
        //tokenize the string
        //
        char *token = strtok(str, " ");
        while (token) {
            args[counter] = token;
            ++counter;
            token = strtok(NULL, " ");
        }
        
        fflush(stdout);
        
        if(strcmp(str,exit2) == 0){
            should_run = 0;
        }
        else{
            pid = fork();
            if (pid == 0)
            {
                //run this when the fork is made
                execv(args[0], args);
                _exit(0);
            }
            else if (pid < 0) {
                //error when forking
                printf("Error forking");
                _exit(0);
            }
            else {
                //the parent process is waiting for the child process to finish
                waitpid(pid, &status, 0);
            }
        }
    }
    return 0;

}
