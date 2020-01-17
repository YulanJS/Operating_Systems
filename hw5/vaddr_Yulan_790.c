#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "There should be two input arguments\n");
		return -1;
	}
	else
	{
		int pageSize = atoi(argv[1]) * 1024;
		int virtualAddr = atoi(argv[2]);
		int pageNum = virtualAddr / pageSize;
		int offset = virtualAddr % pageSize;
		printf("\nVirtual address translation by Yulan790");
		printf("\nPage size = %d, virtual address %d => page number = %d, offset = %d\n",
				pageSize, virtualAddr, pageNum, offset);
		return 0;
	}
}
