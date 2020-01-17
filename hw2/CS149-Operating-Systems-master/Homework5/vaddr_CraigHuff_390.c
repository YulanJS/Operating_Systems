//
//  main.c
//  vaddr
//
//  Created by Craig on 11/20/17.
//  Copyright © 2017 Craig. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFF_SIZE 40
#define BYTE_SIZE 1024

int main(int argc, char *argv[])
{
    char *garbage = NULL;
    long pageSize = 0;
    long address = 0;

    pageSize = strtol(argv[1], &garbage, 0);
    address = strtol(argv[2], &garbage, 0);

    long byteSize = pageSize * BYTE_SIZE;
    long pageNumber = address / byteSize;
    long offset = address - (pageNumber * byteSize);
    printf("Virtual address translation by CraigHuff390\n");
    printf("Page size = %ld, virtual address %ld => page number = %ld, offset = %ld\n", byteSize, address, pageNumber, offset );
    
    return 0;
}

