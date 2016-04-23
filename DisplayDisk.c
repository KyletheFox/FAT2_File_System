#include "FS.h"

int main(int argc, char const *argv[]) {
	
	int i,j;						// Loop Counters
	int fileTracker = 0;			// Where the program is located in the file
	int fd;							// File Pointer
	char *map;						// Pointer for memory map
	char temp[2];					// Temp array to convert int into char
	struct stat fileInfo;			// Struct to hold all the File Metadata
	struct FATentry fat[NUM_OF_BLOCKS * sizeof(struct FATentry)];  	// Array to hold FAT
	char blockArray[NUM_OF_BLOCKS][SIZE_OF_BLOCKS + 1];				// Array to hold Blocks

	// Opens File
	fd = open("FS.txt", O_RDWR);

	// Gets file info and place into struct
	fstat(fd, &fileInfo);
	
	// Creates Memory Map of File
	map = (char*)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);


	// --------------- Printing FAT ------------------------------

	printf("---FAT----\n");

	for (i = 0, j = 0; i < (NUM_OF_BLOCKS * 2); i+=2, j++)	{
		fat[j].link = map[i] * 100;
		fat[j].link += map[i+1];
		fileTracker+=2;
		printf("-------Entry: Link:%d\n", fat[j].link);
	}

	// -------------------------------------------------------------

	// --------------- Printing Data Blocks -------------------------

	printf("---Data Blocks---\n");

	for (i = 0; i < NUM_OF_BLOCKS; ++i) 	{
		
		for (j = 0; j < SIZE_OF_BLOCKS; ++j) {
			
			if (map[fileTracker] < 10) {
				sprintf(temp, "%d", map[fileTracker]);
				blockArray[i][j] = temp[0];
			}
			else {
				blockArray[i][j] = map[fileTracker];
			}
			
			//blockArray[i][j] = map[fileTracker];
			fileTracker++;
		}

		blockArray[i][SIZE_OF_BLOCKS] = '\0';
		printf("---------Block:%s\n", blockArray[i]);

	}

	// --------------------------------------------------------------


	// ---------------- Housecleaning -------------------------------

	munmap(map, fileInfo.st_size);
	close(fd);
	return 0;

	// --------------------------------------------------------------
}