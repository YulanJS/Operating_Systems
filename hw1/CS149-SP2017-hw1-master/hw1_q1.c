#include<stdio.h>
#include<unistd.h>
int main()
{	
	
	int i;
	int pid;
	int cpid;
	int ppid;	
	for(i =0; i < 4; i++)
	{	
		
		cpid = fork();
		pid = getpid();
		ppid = getppid();
		printf("cpid = %d pid = %d ppid = %d", cpid,pid,ppid);
		

	}		
	
	sleep(10);

	return 0;
	
}
