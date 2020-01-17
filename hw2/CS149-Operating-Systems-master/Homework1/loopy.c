/*
 * loopy.c
 *
 *  Created on: Sep 3, 2017
 *      Author: craig
 */

#include<stdio.h>
#include<unistd.h>
int main() {
int i = 0;

if (fork() == 0) {
	++i;
	if (fork() != 0){
		i = 3;

	}
	else
			++i;
	printf("%d ",i);
	fork();
}
else {
	i = 5;
	if (fork() == 0)
		++i;
	printf("%d ",i);
}
return 0;
}

