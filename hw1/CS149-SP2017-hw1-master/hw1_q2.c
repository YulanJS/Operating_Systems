#include<stdio.h>
#include<unistd.h>
#include<time.h>
#define MAXLINE 80 /* max line in command line */

int main(int argc, char *args[])
{
	int maxArgs = MAXLINE/2 + 1;
	*args[maxArgs];/*command line with max 40 args*/
	
	int  should_run = 1;
	printf("CS149 Shell from Kaya Ota\n");
	char in[maxArgs];
	int terminate_condition = 0; 
	int i = 0;
	while(should_run)
	{	
		printf("Kaya-403>");
		fflush(stdout);
		fgets(args,maxArgs,stdin); 
		//char* token = strtok(args, " ");
  		printf("word = %s\n", args);
  		
		printf("after word\n");
		//int index;
		//char* p;
		//index = 0;
		printf("before loop\n");
		//p = strtok(in, " ");
		
		
		
		if(strcmp(args[0],"exits")== 0)
		{	
			printf("ready to terminate\n");
			should_run = 1; //ready to terminate 
		}
		else
		{   
			printf("continue\n");
			/*
			pid = fork();
			if(pid < 0)
			{
				printf("Error: could not make a childe process");



			}
			else if(pid == 0)
			{//child process
				execvp(args[0],args);
								
			}else if(pid > 0)
			{//parent process
				//*if(argv contains "&" at the end)
				//{
					//break; go back to while(should_run)
				//}
				else
				{
					wait();
				}
				
				
	

			}*/
		}//if_strcmp	
		//strverscmp (", "no digit")
		//printf("Enter your name:");
		//fgets(name, sizeof(name), stdin);
		/* after readung user input, the steps are:
		(1) fork a child process using fork()
		(2) the child process will invoke execvp()
		(3)if command include &, parent will NOT invoke wait()
		*/
		
		

	}
	return 0;
}
/*bool isTerminate(char* args[])
{

	
}

*/
