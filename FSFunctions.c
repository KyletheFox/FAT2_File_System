#include "FS.h"

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
	char name[13];								
	
	unsigned int i;						// Loop Counter
	int index = startIndex;		// Index of File

	

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

void parseFileName(char *pathname, char **parsedName) {

	char *token;		// Temporary place holder for token strings
	//char somename[20] = "root/pir/awesome";
	int i = 0;

	printf("%s\n", pathname);
	token = strtok(pathname, "/"); 

	while (token != NULL) {
		printf("Here\n");
		//*(parsedName+i) = token;
		printf("%s\n", token); 
		token = strtok(NULL,"/");
		i++;
	}
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

// Functions needed to be developed
/*
	int findNextFreeBlock()
	--- Uses linear search to find next free block and returns
	--- block number

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


