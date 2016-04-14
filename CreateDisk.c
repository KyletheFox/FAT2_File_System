#include "FS.h"

int main(int argc, char const *argv[]) {
	
	FILE *fp;
	char tmp;
	int i, j;
	struct FileHead root;

	int k = 1000;
	int m = 10000000;
	int n = 1000;
	int o = 1000;

	// Root Directory
	root.type = 5678;
	root.name = convertFileName(ROOT_NAME);
	root.lastAccess = 4512;			// mmddyyyy
	root.blockNum = 5656; 			// Block 0
	root.size = 2359;				// Number of Files/dirs

	// Open File
	fp = fopen("FS.txt", "w+");

	// Filling FAT Table
	for (i = 0; i < NUM_OF_BLOCKS; ++i) {
		if (i == 0) {
			fputc(0x00, fp);
			fputc(0x00, fp);
		}
		else {
			fputc(0x99, fp);
			fputc(0x99, fp);
		}
	}

	// Filling in all the data blocks
	for (i = 0; i < NUM_OF_BLOCKS; ++i)	{

		// Fills an individual block
		for (j = 0; j < SIZE_OF_BLOCKS; ++j) {
				
				// Creates Root Directory in first block
				if (i == 0) {
					if (j >= 0 & j <= 3) {
						fprintf(fp, "%c", root.type/k);
						root.type %= k;
						k /= 10;
					}
					if (j == 4)	
						fprintf(fp, "%s", root.name);

					if (j >= 16 && j <= 23) {
						fprintf(fp, "%c", root.lastAccess/m);
						root.lastAccess %= m;
						m /= 10;
					}
					if (j >= 24 & j <= 27) {
						fprintf(fp, "%c", root.blockNum/n);
						root.blockNum %= n;
						n /= 10;
					}
					if (j >= 28) {
						fprintf(fp, "%c", root.size/o);
						root.size %= o;
						o /= 10;
					}
				}

				else 
					fputc(0, fp);
				
		}
	}

	free(root.name);
	fputc(EOF, fp);
	fclose(fp);
	return 0;
}