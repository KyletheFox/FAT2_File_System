#include "FS.h"

/*
	Creating a file/dir

	1. Make a new FileHeader
		- Find next Free Block
		- Set lastAccess
		- Set size
		- Fill in Name and Type
	
	2. Place into File Structure
		- Go to dir using parse pathname
		- Search File Header starting @ ROOT
			* If not found file is not created
		- Insert Header
		- Update FAT
*/

int koCreate(unsigned int type, char* pathname) {

	int fp;
	char **parsePath;
	char *fileName;
	char *temp;
	char *map;
	int fileIndex = 0;
	int blockNum;
	int date;
	int index;
	struct FileHead newHead;
	struct tm *timeStamp;
	struct stat fileInfo;

	fp = open("FS.txt", O_RDWR);	// Opening disk

	// Gets file info and place into struct
	fstat(fp, &fileInfo);

	// Memory Mapped File
	map = (char*)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fp, 0);

	// Get parsed pathname
	parsePath = parsePathName(pathname);

	// loop to get last element of pointer array
	while (temp != NULL) {
		temp = parsePath[fileIndex];
		//printf("%d %s\n", fileIndex, temp);
		if(temp == NULL) 
			fileIndex--;
		else
			fileIndex++;
	}

	// Get File Name
	fileName = parsePath[fileIndex];

	// Remove Filename from parsePath to dir block to write filehead
	parsePath[fileIndex] = NULL;

	printf("This should be null: %s\n", parsePath[fileIndex]);

	// Get Time Stamp
	timeStamp = getTimeStamp();

	// Fill in date from timeStamp
	date = (timeStamp->tm_mon+1) * 1000000;
	date += timeStamp->tm_mday * 10000;
	date += timeStamp->tm_year + 1900;

	// Get First Free Block
	blockNum = getNextFreeBlock(map);

	// Update FAT table
	updateFAT(blockNum, map, 0000);

	// Find Data block associated with the path
	index = findFile(parsePath, map);	

	printf("index: %d\n", index);

	// Create Struct
	newHead.type = 2;
	newHead.name = convertFileName(fileName);
	newHead.lastAccess = date;
	newHead.blockNum = blockNum;
	newHead.size = 0;

	// Write to File System
	writeHeader(newHead, map, index);

	return 0;

}

/*
	Open a file/dir

	1. Go to first data block of file
		- Search through dir to find the file
		- Return the index of the data block the file
*/

/* 
	Delete a file/dir

	1. Remove File header from directory structure
		- Go to the file block that contains the file header
		- Overwrite with 0's
		- Update FAT
*/

/*
	Close file

	1. Return 0
*/

/*
	Read File

	1. Return char from block at location x	

	* Like getc - reads one char at a time
*/

/*
	Write File

	1. Set char in data block at location x
	2. Fills in rest of chars after to zero

	* Like putc - writes on char at a time
*/