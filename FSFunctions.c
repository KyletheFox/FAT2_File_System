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
	
	int i;						// Loop Counter
	int index = startIndex;		// Index of File

	

	for (i = 0; i < sizeof(struct FileHead); ++i) {

		if (i >= 0 && i <= 3) {
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