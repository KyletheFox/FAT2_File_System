#include "FS.h"

/*
	This program initalizes a file system by creating a FAT table,
	initalizing the root directory, and all the data blocks.
	This program builts the file system using the specs definided 
	in FS.h. 

	*- ONLY LOGIC in this file. Change values in FS.h to change default
		build size -*
*/

int main(int argc, char const *argv[]) {
	
	//  ------- Variables ------------------------------------
	FILE *fp;				// Disk File pointer
	int i, j;				// Loop Counters
	int date;				// Placeholder for root dir lastAccess
	struct FileHead root;	// Root Dir file info
	struct tm *timeStamp;	// System timestamp

	/* 
		Used to print out characters starting from the front.
		Uses % and /10 to accomplish this. 
			1000 - 		4 chars
			10000000 - 	8 chars
	*/
	int k = 1000;
	int m = 10000000;
	int n = 1000;
	int o = 1000;
	// --------------------------------------------------------

	// --------- Getting Timestamp ----------------------------

	// Get Time Stamp
	timeStamp = getTimeStamp();

	// Fill in date from timeStamp
	date = (timeStamp->tm_mon+1) * 1000000;
	date += timeStamp->tm_mday * 10000;
	date += timeStamp->tm_year + 1900;

	// ------------------------------------------------------------

	// Root Directory Struct
	root.type = 2;							// Type of header
	root.name = convertFileName(ROOT_NAME);	// Dir name
	root.lastAccess = date;				// mmddyyyy
	root.blockNum = 0; 					// Block 0
	root.size = 0;						// Number of Files/dirs

	// Open/Creates File System File
	fp = fopen("FS.txt", "w+");

	// ----- Filling FAT Table ------------------------------------
		/*
			For the first FAT entry, fill in block number with two
			chars 0000. All the others entries get filled in with FFFF

			** Look in FS.h for ENTRY numbers meanings **
		*/

	for (i = 0; i < NUM_OF_BLOCKS; ++i) {
		if (i == 0) {
			fputc(00, fp);
			fputc(00, fp);
		}
		else {
			fputc(99, fp);
			fputc(99, fp);
		}
	}
	// -----------------------------------------------------------

	// -------- Filling in all the data blocks -------------------

	// Loop through all the data blocks
	for (i = 0; i < NUM_OF_BLOCKS; ++i)	{

		// Fills an individual block
		for (j = 0; j < SIZE_OF_BLOCKS; ++j) {
				
				// Creates Root Directory in first block
				// *** Only for ROOT dir in block 0 *****
				if (i == 0) {
					
					// Inserting File type
					if (j >= 0 && j <= 3) {
						fprintf(fp, "%c", root.type/k);
						root.type %= k;
						//printf("root.type: %u\n", root.type);
						k /= 10;
					}

					// Inserting File Name
					if (j == 4)	
						fprintf(fp, "%s", root.name);

					// Inserting File Last Access Time
					if (j >= 16 && j <= 23) {
						fprintf(fp, "%c", root.lastAccess/m);
						root.lastAccess %= m;
						//printf("root.lastAccess: %u\n", root.lastAccess);
						m /= 10;
					}

					// Inserting File Block Number
					if (j >= 24 && j <= 27) {
						fprintf(fp, "%c", root.blockNum/n);
						root.blockNum %= n;
						//printf("root.blockNum: %u\n", root.blockNum);
						n /= 10;
					}

					// Inserting File Size
					if (j >= 28 && j <= 31)  {
						fprintf(fp, "%c", root.size/o);
						root.size %= o;
						//printf("root.size: %u\n", root.size);
						o /= 10;
					}
				}

				else {
					// Fill in rest of block and all other blocks with zeros
					fputc(0, fp);
				}
				
		}
	}
	// --------------------------------------------------------------

	// House Keeping
	free(root.name);
	fputc(EOF, fp);
	fclose(fp);

	return 0;
}