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

	// Variable
	int fp;					// File pointer					
	char **parsePath;		// Holds array of tokened pathname
	char *fileName;			// Filename of the file to create
	char *temp;				// Temporary Placeholder used to get last parsed path
	char *map;				// memory map of the disk
	int fileIndex = 0;		// Index of where the filename is in the parsedPath
	int blockNum;			// The block number of the file to be created
	int date;				// Holds the info about the current date
	int index;				// The index of the block number rep by the file 
	int i;					// Loop counter
	struct FileHead newHead;	// The new file header to be inserted
	struct tm *timeStamp;		// Holds data from the system timestamp
	struct stat fileInfo;		// Holds metadata about the file to be opened

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
		printf("%d %s\n", fileIndex, temp);
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

	// Find Data block associated with the path
	index = findFile(parsePath, map);	
	
	printf("index: %d\n", index);

	if (index != -1) {
		// Create Struct
		newHead.type = 2;
		newHead.name = convertFileName(fileName);
		newHead.lastAccess = date;
		newHead.blockNum = blockNum/2;
		newHead.size = 0;

		// Write to File System
		if (writeHeader(newHead, map, index) != -1) {
			// Update FAT table
			updateFAT(blockNum, map, 0000);

			// Clear Block of any left over data
			for (i = SIZE_OF_FAT + newHead.blockNum*SIZE_OF_BLOCKS; 
					i < SIZE_OF_FAT + newHead.blockNum*SIZE_OF_BLOCKS + SIZE_OF_BLOCKS;
					i++) {
				printf("%d\n", i - SIZE_OF_FAT);
				map[i] = 0;
			}

			return 1;
		}
	}

	// File was not created
	else {
		return -1;
	}
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

int koDelete(char *pathname) {

	// Variables 
	int fp; 					// File Pointer
	int index;					// Holds location of file
	int fileIndex = 0;			// Index of where the filename is in the parsedPath
	int i = 0, j = 0;						// Loop counter
	char **parsePath;			// Holds array of tokened pathname
	char *map;					// memory map of the disk
	char *fileName;			// Filename of the file to create
	char *temp;					// Temporary Placeholder used to get last parsed path
	struct stat fileInfo;		// Holds metadata about the file to be opened
	struct FileHead tempHead;		// Place holder for file headers in data block


	fp = open("FS.txt", O_RDWR);	// Opening disk

	// Gets file info and place into struct
	fstat(fp, &fileInfo);

	// Memory Mapped File
	map = (char*)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fp, 0);

	// Get parsed pathname
	parsePath = parsePathName(pathname);

	// loop to get last element of pointer array
	do {
		temp = parsePath[fileIndex];
		printf("%d %s\n", fileIndex, temp);
		if(temp == NULL) 
			fileIndex--;
		else
			fileIndex++;
	} while (temp != NULL);

	// Get File Name
	fileName = parsePath[fileIndex];

	printf("%s\n", fileName);

	// Remove Filename from parsePath to get index of block where file header is located
	parsePath[fileIndex] = NULL;

	// Gets index of the bloch where the file header to delete is 
	index = findFile(parsePath, map);

	while (i <= SIZE_OF_BLOCKS) {
		tempHead = GetFileHead(map, index, &tempHead);
		if(strcmp(convertFileName(fileName), tempHead.name) == 0) {
			
			// Clear the file header
			for (j = index; j < SIZE_OF_FILEHEAD + index; ++j) {
				map[j] = 0;
			}

			// Free Block Allocated by file header
			updateFAT(tempHead.blockNum*2, map, 9999);

			// Return Good 
			return 1;
		}
		else {
			i += SIZE_OF_FAT;
			index += SIZE_OF_FAT;
		}
	}

	// Return unsucessful
	return -1;

}

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