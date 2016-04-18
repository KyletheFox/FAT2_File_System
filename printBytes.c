#include "FS.h"

int main(int argc, char const *argv[])
{
/*
*	Objectinve read log file header inside block
*/

	int fp;							// File Descriptor
	int i;							// Loop Counters
	int startIndex = SIZE_OF_FAT;	// Index of the first char in the first db
	int readBytes = 1024;			// Number of Bytes to print out
	struct stat fileInfo;			// Struct to hold all the File Metadata
	char *map;						// Pointer for memory map  

	fp = open("FS.txt", O_RDONLY);	// Opening disk

	// Gets file info and place into struct
	fstat(fp, &fileInfo);

	// Memory Mapped File
	map = (char*)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fp, 0);

	// ------ Loop through number of bytes to print out -----
	for (i = startIndex; i < startIndex + readBytes; ++i)	{

		/*	
			---- Use for printing out a File Header ----

		if (i >= startIndex+4 && i < startIndex+17) {
			printf("%c", map[i]);
		}
		else {
			printf("%d", map[i]);
		}
		*/

		printf("%d", map[i]);

	}
	// -------------------------------------------------------


	// ------- House Keeping ----------------------
	printf("\n");
	close(fp); 
	return 0;
	// --------------------------------------------
}