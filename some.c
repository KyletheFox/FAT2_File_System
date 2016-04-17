#include "FS.h"

int main(int argc, char const *argv[])
{

/*
*	Objectinve read log file header inside block
*/

	int fp;
	int i;
	int startIndex = SIZE_OF_FAT;
	int readBytes = 128;
	struct stat fileInfo;			// Struct to hold all the File Metadata
	char *map;						// Pointer for memory map  

	fp = open("FS.txt", O_RDONLY);	// Opening disk

	// Gets file info and place into struct
	fstat(fp, &fileInfo);

	// Memory Mapped File
	map = mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fp, 0);

	for (i = SIZE_OF_FAT; i < SIZE_OF_FAT + readBytes; ++i)	{
		//printf("%d\n", i);
		/*
		if (i >= 32+4 && i < 32+17) {
			printf("%c", map[i]);
		}
		else {
			printf("%d", map[i]);
		}
		*/

		printf("%c", map[i]);

	}


	// House Keeping
	printf("\n");
	close(fp); 

	return 0;
}