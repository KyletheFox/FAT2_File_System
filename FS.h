
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	char *name;
	int lastAccess;
	int blockNum;
	int size;
} FileHead;

#define NUM_OF_BLOCKS 4
#define SIZE_OF_BLOCKS 32
#define SIZE_OF_FAT NUM_OF_BLOCKS * sizeof(struct FATentry)

#define ROOT_BLOCK 0		// blocks[ROO/T_BLOCK][x]
#define ROOT_NAME "ROOT"

#define MAX_FILE_NAME_SIZE 12

char *convertFileName(char *givenName);
struct FileHead GetFileHead(char *disk, int startIndex, struct FileHead *output);


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