#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct FATentry{
	int link;
} FATentry;

typedef struct FileHead {
	int type;
	char name[12];
	int lastAcces;
	int blockNum;
	int size;
} FileHead;

#define NUM_OF_BLOCKS 4
#define SIZE_OF_BLOCKS 32
#define SIZE_OF_FAT NUM_OF_BLOCKS * sizeof(struct FATentry)

#define ROOT_BLOCK 0		// blocks[ROOT_BLOCK][x]
#define ROOT_NAME "ROOT"



/*	
	Specs:

	File Header:
		Type 			4B
		Name			12B
		Last Access		8B	(ddmmyyyy)
		Block Number    4B
		File Size 		4B
------------------------------------------
		Total: 			32B 
*/