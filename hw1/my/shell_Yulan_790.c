/**
 * Simple shell interface program.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <readline/readline.h>
//#include <readline/history.h>

#define MAXLINE		80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAXLINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int shouldRun = 1;
	int i;
	char userInput[MAXLINE];
	char* token;


	pid_t pid;
	printf("\nCS149 Shell from Yulan Jin");
    while (shouldRun){
    	fflush(stdout);
    	i = 0;
    	int shouldWait = 1;//boolean wait or not
    	int status;
        printf("\nYulan-790>");
        fgets(userInput, MAXLINE + 1, stdin);//fgets: last token has a "\n"

        //printf("\n%s\n", userInput);
        args[i] = strtok(userInput, " "); //use " " to parse?

        //printf("\n%s", args[i]);
        while(args[i] != NULL)
        {
        	//printf("\n%d", i);
        	i++;
        	args[i] = strtok(NULL, " ");
        	//printf("\n%s", args[i]);
        }
        /*
        printf("\nmyArgs");
        for(int j = 0; j < i; j++)
        {
        	printf("\n%s", args[j]);
        }

        printf("\n%d\n", i);
        */
        //because of fgets, need to remove \n in the last token
        args[i - 1] = strtok(args[i - 1], "\n");
        if(strcmp(args[i - 1], "&") == 0)
        {
        	args[i - 1] = NULL;
        	memset(&args[i], 0, 1);
        	shouldWait = 0;
        }
        if((strcmp(args[0], "exit") == 0) && (args[1] == NULL))
		{
        	//printf("shouldRun = 0");
			shouldRun = 0;
			break;//exit should use break
		}

        // i the last index; let child procedure deal with empty input
        /*
        for(int j = 0; j <= i; j++)//args[i] is null
        {
        	printf("\n%s\n", args[j]); //null is not %s segmentation error
        }
	    */
        //printf("\nstart forking");
        pid = fork(); //each time fork, creating a new child process
        if(pid < 0)
        {
        	printf("\nFork Error");
        	exit(1);
        }
        else if(pid == 0)
        {
        	//child process
        	//printf("\nI am the child");
			//fflush(stdout); 
			// use fflush after printf to make sure strings in I/O buffer won't get lost in exec
        	if(execvp(args[0], args) == -1)
        	{
        		printf("\nExecvp error");
        		memset(&args[0], 0, MAXLINE/2 + 1);
        		exit(1);
        	}
        	printf("\nI am done");
        	memset(&args[0], 0, MAXLINE/2 + 1); //clear first i elems, enough
        	exit(0);
        }
        else
        {
        	//parent process
        	//printf("\nThis is the parent");
        	//printf("\ni is %d", i);
        	//printf("\n args[i - 1] is %s", args[i - 1]);
        	//if(strcmp(args[i - 1], "&") != 0)
			if(shouldWait)
			{
				//run simultaneously
				//printf("\nShould Wait");
				//wait returns child's pid
				//Why wait for any child doesn't work?
				while(wait(&status) != pid);
			}

        	//printf("\nchild done");

        }
        /*

       		else
       		{
       			//wait for child
       		}
               */

        //after all finished




        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    }
    //printf("\nall done\n");
	return 0;
}
