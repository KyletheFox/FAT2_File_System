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
	int type = 0;
	int lastAccess = 0;
	int blockNum = 0;
	unsigned int size = 0;	
	char *name;								
	
	int i;						// Loop Counter
	int index = startIndex;		// Index of File

	//printf("%d\n", disk[4]);

	for (i = 0; i < sizeof(struct FileHead); ++i) {
		if (i >= 0 && i <= 3) {
			output->type += disk[index];
			if (i < 3)
				output->type *= 10;				
		}

		if (i >= 4 && i <= 15) {
			//output->name[i-4] = disk[index];
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

		if (i >= 28) {
			output->size += disk[index];
			if (i < 31)
				output->size *= 10;
		}

		index++;
	}

	//name[MAX_FILE_NAME_SIZE] = '\0';

	printf("Type: %d\n", output->type);
	//printf("Name: %s\n", output->name);
	printf("lastAccess: %d\n", output->lastAccess);
	printf("blockNum: %d\n", output->blockNum);
	printf("size: %d\n", output->size);

	return *output;

}