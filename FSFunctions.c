#include "FS.h"

using namespace std;

char *convertFileName(char *givenName) {

	int length = strlen(givenName);
	int i;
	char *sizedName;

	// Allocate memory in heap for new char *
	sizedName = (char *)malloc(sizeof(char)*MAX_FILE_NAME_SIZE + 1);

	for (i = 0; i < MAX_FILE_NAME_SIZE; ++i) {
		if (i < (MAX_FILE_NAME_SIZE - length))
			sizedName[i] = '$';
		else
			sizedName[i] = givenName[i - (MAX_FILE_NAME_SIZE - length) ];
	}

	// Add eos
	sizedName[MAX_FILE_NAME_SIZE] = '\0';
	return sizedName; 

}

// Does not move the file pointer since using mmap)
struct FileHead GetFileHead(char *disk, int startIndex, struct FileHead *output) {

	// Data Fields for FileHead struct
	output->type = 0;	
	output->lastAccess = 0;
	output->blockNum = 0;
	output->size = 0;
	char *name;								
	
	unsigned int i;						// Loop Counter
	int index = startIndex;		// Index of File

	name = (char*)malloc(13);

	for (i = 0; i < sizeof(struct FileHead); ++i) {

		if (i <= 3) {
			output->type += disk[index];
			if (i < 3)
				output->type *= 10;				
		}

		if (i >= 4 && i <= 15) 	{
			name[i-4] = disk[index];
		}

		if (i >= 16 && i <= 23) {
			output->lastAccess += disk[index];
			if (i < 23)
				output->lastAccess *= 10; 
		}

		if (i >= 24 && i <= 27) {
			output->blockNum += disk[index];
			if (i < 27)
				output->blockNum *= 10;
		}

		if (i >= 28 && i <= 31) {
			output->size += disk[index];
			if (i < 31)
				output->size *= 10;
		}

		index++;
	}

	name[MAX_FILE_NAME_SIZE] = '\0';
	output->name=name;

	printf("Type: %d\n", output->type);
	printf("Name: %s\n", output->name);
	printf("lastAccess: %d\n", output->lastAccess);
	printf("blockNum: %d\n", output->blockNum);
	printf("size: %d\n", output->size);

	return *output;

}

/*
	Returns array of strings based on pathname given as argument.
	Splits up pathname by "/" and inserts each string into the 
	array of strings. 
*/
char **parsePathName(char pathname[]) {

	char *token;		// Temporary place holder for token strings
	char **retVal;		// Array of the each part of the pathname
	int i = 0;			// Loop counter

	// Allocate memory on heap for retVal
	retVal = (char**)malloc(strlen(pathname)*MAX_DIR_DEPTH + 1);

	// Get first dir of input -- Should be ROOT
	token = strtok(pathname, "/"); 

	// Loop through given pathname and separate by "/"
	while (token != NULL) {
		retVal[i] = token; 
		token = strtok(NULL,"/");
		i++;
	}

	return retVal;
}

/*
	This function gets the time of day to fill in time stamp for
	file headers. Creates memory on heap for struct tm pointer. 
	Must free space at before closing file

	tm_year: year since 1900
	tm_mon: month of the year
	tm_mday: day of the month
*/
struct tm *getTimeStamp() {

	// Variables
	struct tm *timeStamp;  	// Date Obj
	time_t timev;			// time object need to get tm obj

	// Allocate memory on heap
	timeStamp = (tm*)malloc(sizeof(struct tm)); 

	// Get the systems timestamp
	time( &timev );

	// Convert to the local time and fill in tm obj
	timeStamp = localtime(&timev);

	// Returns pointer to struct tm in the heap
	return timeStamp;
}

/*
	Uses map to loop through FAT and find the next free block. 
	If there is a free block, the value of the FAT table for that 
	block will have the value of 9999. This will be in the FAT as
	two chars, both with the value 99 (0x63 in hex). Function will
	return value of the next free block. If there are no free blocks
	function will return -1.
*/
int getNextFreeBlock(char *map) {

	int freeBlock;		// Index of free block
	int i;				// Loop Counter

	for(i = 0; i < SIZE_OF_FAT; i+=2) {
		if(map[i] == 99) {
			freeBlock = i;
			return freeBlock;	// Sucess
		}
	}

	return -1; 	// Free block not found

}

void updateFAT(int fatIndex, char*  map, int newValue) {
	int first = newValue/100;
	int second = newValue%100;

	map[fatIndex] = first;
	map[fatIndex + 1] = second;
}

/*
	Finds the index of the first character for the file header
	belonging to the file path given
*/
int findFile(char **parsePath, char* map) {

	// Variables
	int index = SIZE_OF_FAT;		// Index where the file header is
	int i=0, j=0;						// Loop Counters
	struct FileHead temp; 			// Temporay placeholder to hold values of File Header
	char found = 0;				// Flag to determine if the file header was found

	while (parsePath[i] != NULL && j < SIZE_OF_BLOCKS) {
		temp = GetFileHead(map, index, &temp);
		printf("%s\n", temp.name);
		printf("%s\n", convertFileName(parsePath[i]));
		if (strcmp(convertFileName(parsePath[i]), temp.name) == 0) {
			printf("Found %s\n", temp.name);
			index = temp.blockNum*SIZE_OF_BLOCKS + SIZE_OF_FAT;
			i++;
		} else {
			index += SIZE_OF_FILEHEAD;
			j += SIZE_OF_FILEHEAD;
			printf("No Match\n");
		}
	}

	if (index >= SIZE_OF_BLOCKS) {
		return -1;
	}
	else {
		return index;
	}
}

// Functions needed to be developed
/*

	int findNextLinkBlock()
	--- Determins if there is a link to another block. returns
	--- linked block number on true or -1 if false

	struct FileHead *createFileHead(type, name, size)
	--- Creates a FileHead struct from given info. Function 
	--- automatically gets timestamp and next free block.

	int getFileSize(FILE *)
	--- Counts charaters inside file and returns the count

	int findFile(pathname)
	--- Searches through file system for the file. Returns the
	--- index of the file block else returns -1

	int goToBlockIndex( block number )
	--- Gets the index of the specified block number

	
*/


