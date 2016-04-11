#include "FS.h"

typedef struct FileHeadRoot {
		int type = 2;
		char name[12] = ROOT_NAME;
		int lastAcces = 0000;
		int blockNum = ROOT_BLOCK;
		int size = 0;
	}

int main(int argc, char const *argv[]) {
	
	FILE *fp;
	char tmp;
	int i, j;
	struct FileHeadRoot root;
	

	// Open File
	fp = fopen("FS.txt", "w+");

	// Filling FAT Table
	for (i = 0; i < NUM_OF_BLOCKS; ++i) {
		fputc(0, fp);
	}

	for (i = 0; i < NUM_OF_BLOCKS; ++i)	{
		for (j = 0; j < SIZE_OF_BLOCKS; ++j) {
				if (i == 0) {
					if (j == 0)
						fprintf(fp, "%d", root.type);
					if (j == 5)	{
						fprintf(fp, "%s", root.name);
					if (j == 17)
						fprintf(fp, "%d", root.lastAcces);
					if (j == 21)
						fprintf(fp, "%d", root.blockNum);
					}
				}
		}
	}

	fputc(EOF, fp);
	fclose(fp);
	return 0;
}