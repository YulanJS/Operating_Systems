/*
 * memalloc.c
 *
 *  Created on: Sep 3, 2017
 *      Author: craig
 */
#include <stdlib.h>

int main3()
{
	int num = 0, *ptr;

	//size = 100*sizeof(int);

	ptr = (int*) calloc(100, sizeof(int));

	while(num < 100){
		ptr[num] = num;
		++num;
	}

	for(int i = 0; i < 100; i++)
		{
			printf("%d ", ptr[i]);
		}

	free(ptr);
	return 0;
}


