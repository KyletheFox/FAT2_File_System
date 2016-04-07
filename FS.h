#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct FATentry{
	char next;
	char block;
} FATentry;

#define NUM_OF_BLOCKS 4
#define SIZE_OF_BLOCKS 10
#define SIZE_OF_FAT NUM_OF_BLOCKS * sizeof(struct FATentry)

#define ROOT_BLOCK 0		// blocks[ROOT_BLOCK][x]



/*	
	Specs:

	File Header:
		Type 			4B
		Name			12B
		Last Access		8B	(ddmmyyyy)
		Block Number    4B
		Block Size 		4B
------------------------------------------
		Total: 			32B 
*/