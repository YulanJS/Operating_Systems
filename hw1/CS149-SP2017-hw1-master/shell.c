/*Submit Version*/
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAXLINE 80 /*max length of command prompt*/

int main(void)
{
	/*defintions */
	int maxArgs = MAXLINE/2 + 1;
	char* args[maxArgs]; /*command line with max 40 argments*/
	//int arg_i = 0; 
	int should_run = 1; /*flag for termination 1 = true, 0 = false*/
	char command[maxArgs];
	int pid; //holds process id  
	char* p;
	int index = 0;
	int and_sign;//boolean value if the command contains &-notation
	char* last_word = " ";
	int length = 0;
	int counter = 0;
	printf("CS149 Shell from\n");
	while(should_run == 1)
	{
		//printf("---------should_run: %d \n--------\n", should_run);
		int arg_i = 0; 
	
		printf("Demo-123>");
		fflush(stdout);
		
		fgets(command,maxArgs,stdin); 

		if (strcmp(command, "exit\n") == 0)
		{
			should_run = 0;
			printf("Bye.\n");
			printf("should_run: %d\n", should_run);
			break;
		}
		else
		{
			printf("Continue\n");
			p = strtok(command, " "); 
			//printf("before loop\n");
			while(p != NULL)
			{	
				//make command array
				if (strcmp(p, "&\n") == 0)
				{
					and_sign = 1;
					p = strtok(NULL," ");
					
				}
				else
				{
					args[arg_i++] = p; 
					p = strtok(NULL, " ");
					index++;
					counter++;
				}

			}//while p != NULL
				
				last_word = args[arg_i-1] ;
				length = strlen(last_word);
				last_word[length - 1] = '\0';
				args[arg_i] = last_word;
				args[arg_i++] = NULL;
				int i =0;
				int len = 0; 
				/*while(i < index)
				{
					printf("%d\n", len);
					len = strlen(args[i]);
					printf("args[%d]:%s %d: \n",i, args[i],len);
					i++;
				}//while*/
				/*for (i = 0; i < counter; i++)
				{
					printf("%d\n", len);
					len = strlen(args[i]);
					printf("args[%d]:%s %d: \n",i, args[i],len);
				}*/

				pid = fork();/*Create a new process*/
				if(pid < 0)
				{
					printf("ERROR: could not create a child process\n");
				}
				else if(pid == 0)
				{
					//printf("Children_Process\n");
					if(execvp(args[0],args) == -1)
					{
						printf("ERROR: should_run %d\n", should_run );
						printf("Error: can not execute your command: %s \n", command);
						/*if(strcmp(command, "exit\n")== 0)
						{
							should_run = 0;
							printf("Bye. \n");
							printf("? UPDATE ?: should_run: %d\n", should_run);	
						}
						else
						{
							printf("REMAIN: should_run = %d\n", should_run);
						}*/


					}
					else
					{
						printf("Success\n");
					}
				}
				else if(pid > 0)
				{
					//printf("Parent_Process\n");
					if(and_sign == 1)
					{
						and_sign = 0;
						
					}
					else
					{
						and_sign = 0;
						wait();
					}
				}	
		}//if-strcmp
		printf("END: should_run %d \n",should_run );
		
	}//while







	return 0;
}//main