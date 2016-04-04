#include "FS.h"

// This program will create the "disk"

int main(int argc, char const *argv[]) {
	
	FILE *fp;
	char tmp;
	int i, j;

	fp = fopen("FS.txt", "w+");

	// Filling FAT Table
	for (i = 0; i < NUM_OF_BLOCKS; ++i) {
		fputc(i, fp);
		fputc(0, fp);
	}

	for (i = 0; i < NUM_OF_BLOCKS; ++i)	{
		for (j = 0; j < SIZE_OF_BLOCKS; ++j) {
			fputc(0, fp);
		}
	}

	fputc(EOF, fp);
	fclose(fp);
	return 0;
}