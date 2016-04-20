#include <ctime>
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
	unsigned int type;
	char *name;
	unsigned int lastAccess;
	unsigned int blockNum;
	unsigned int size;
} FileHead;

#define NUM_OF_BLOCKS 16
#define SIZE_OF_BLOCKS 64
#define SIZE_OF_FAT NUM_OF_BLOCKS * 2
#define SIZE_OF_FILEHEAD 32


#define ROOT_BLOCK 0		// blocks[ROO/T_BLOCK][x]
#define ROOT_NAME "ROOT"

#define MAX_FILE_NAME_SIZE 12
#define MAX_DIR_DEPTH 10

char *convertFileName(char *givenName);
struct FileHead GetFileHead(char *disk, int startIndex, struct FileHead *output);
char **parsePathName(char pathname[]);
struct tm *getTimeStamp();
int getNextFreeBlock(char *map);
void updateFAT(int fatIndex, char* map, int newValue);
int findFile(char **parsePath, char* map);

int koCreate(unsigned int type, char* pathname);


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

/*
	File Header / FAT Entry:

	File Header Types:
		0001 - File
		0002 - Directory

	FAT Links:
		9999 - Free Space
		0000 - Full w/o link
		xxxx - Full w/ link to block xxxx
*/