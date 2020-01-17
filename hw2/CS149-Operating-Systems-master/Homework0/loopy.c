/*
 * loopy.c
 *
 *  Created on: Sep 3, 2017
 *      Author: craig
 */

#include <stdio.h>

int main(){

	int array [100];
	int counter = 0;

	while(counter < 100){
		array[counter] = counter;
		counter++;
	}

	for(int i = 0; i < 100; i++)
	{
		printf("%d ", array[i]);
	}

	main2();
	main3();
	return 0;
}

