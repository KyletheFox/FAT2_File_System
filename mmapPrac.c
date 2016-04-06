#include "FS.h"

int main(int argc, char const *argv[]) {
	
	int i,j;
	int fileTracker = 0;
	int fd;
	char *map;
	char temp[2];
	struct stat fileInfo;
	struct FATentry entry;
	struct FATentry fat[NUM_OF_BLOCKS * sizeof(struct FATentry)];
	char blockArray[NUM_OF_BLOCKS][SIZE_OF_BLOCKS + 1];

	// Opens File
	fd = open("FS.txt", O_RDWR);

	// Gets file info and place into struct
	fstat(fd, &fileInfo);
	
	// Creates Memory Map of File
	map = mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);

	// --------------- Printing FAT ------------------------------

	printf("---FAT----\n");

	for (i = 0, j = 0; i < (NUM_OF_BLOCKS * sizeof(struct FATentry)); i+=2, j++)	{
		fat[j].next = map[i];
		fat[j].block = map[i+1];
		fileTracker+=2;
		printf("-------Entry: Block:%d Link:%d\n", fat[j].next, fat[j].block);
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
			fileTracker++;
		}

		blockArray[i][SIZE_OF_BLOCKS] = '\0';
		printf("---------Block:%s\n", blockArray[i]);

	}

	munmap(map, fileInfo.st_size);

	close(fd);

	return 0;
}