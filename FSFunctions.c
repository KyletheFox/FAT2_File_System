#include "FS.h"

// C++ command
using namespace std;

/*
	Converts filenames so they are all the same size. All 
	Files names are to be 12 chars long. If the given name
	is too small, the front is padded in the front with '$'s.
	If the file name is too long, the last n digits past the
	12th are cut off.
*/
char *convertFileName(char *givenName) {

	int length = strlen(givenName); // Gets the length of the name to convert
	int i;							// Loop Counter
	char *sizedName;				// Formatted Name to return

	// Allocate memory in heap for new char *
	sizedName = (char *)malloc(sizeof(char)*MAX_FILE_NAME_SIZE + 1);

	// Loop that formats the file name
	for (i = 0; i < MAX_FILE_NAME_SIZE; ++i) {
		
		// Pads '$'s to the front of the name if it is smaller than 12
		if (i < (MAX_FILE_NAME_SIZE - length)) {
			sizedName[i] = '$';
		}

		// Places the given name into the formatted name
		else {
			sizedName[i] = givenName[i - (MAX_FILE_NAME_SIZE - length) ];
		}

	}

	// Add eos
	sizedName[MAX_FILE_NAME_SIZE] = '\0';

	return sizedName; 
}

/*
	Gets a File Header from the specified index. Goes through each char
	of the FileHead and inputs them in to a FileHead struct to return
*/
struct FileHead GetFileHead(char *disk, int startIndex, struct FileHead *output) {

	// Variables
	char *name;					// File Header Name			
	unsigned int i;				// Loop Counter
	int index = startIndex;		// Index of File

	// Init Data Fields for FileHead struct
	output->type = 0;	
	output->lastAccess = 0;
	output->blockNum = 0;
	output->size = 0;

	// Allocate data for file header name
	name = (char*)malloc(MAX_FILE_NAME_SIZE + 1);

	// Loop through the chars to insert into struct
	for (i = 0; i < sizeof(struct FileHead); ++i) {

		// Gets the first 4 chars - Type
		if (i <= 3) {
			output->type += disk[index];
			if (i < 3)
				output->type *= 10;				
		}

		// Gets 12 chars - Name
		if (i >= 4 && i <= 15) 	{
			name[i-4] = disk[index];
		}

		// Gets 8 chars - LastAcess
		if (i >= 16 && i <= 23) {
			output->lastAccess += disk[index];
			if (i < 23)
				output->lastAccess *= 10; 
		}

		// Gets 4 chars - blockNum
		if (i >= 24 && i <= 27) {
			output->blockNum += disk[index];
			if (i < 27)
				output->blockNum *= 10;
		}

		// Get 4 chars - Size
		if (i >= 28 && i <= 31) {
			output->size += disk[index];
			if (i < 31)
				output->size *= 10;
		}

		// Increase counter to read next char
		index++;
	}

	// Add eos to name
	name[MAX_FILE_NAME_SIZE] = '\0';

	// Insert Name
	output->name=name;

	return *output;
}

/*
	Returns array of strings based on pathname given as argument.
	Splits up pathname by "/" and inserts each string into the 
	array of strings. 
*/
char **parsePathName(char pathname[]) {

	char *token;		// Temporary place holder for token strings
	char **retVal;		// Array of the each part of the pathname
	int i = 0;			// Loop counter

	// Allocate memory on heap for retVal
	retVal = (char**)calloc(MAX_DIR_DEPTH, strlen(pathname));

	// Get first dir of input -- Should be ROOT
	token = strtok(pathname, "/"); 

	// Loop through given pathname and separate by "/"
	while (token != NULL) {
		retVal[i] = token; 
		token = strtok(NULL,"/");
		i++;
	}

	return retVal;
}

/*
	This function gets the time of day to fill in time stamp for
	file headers. Creates memory on heap for struct tm pointer. 
	Must free space at before closing file

	tm_year: year since 1900
	tm_mon: month of the year
	tm_mday: day of the month
*/
struct tm *getTimeStamp() {

	// Variables
	struct tm *timeStamp;  	// Date Obj
	time_t timev;			// time object need to get tm obj

	// Allocate memory on heap
	timeStamp = (tm*)malloc(sizeof(struct tm)); 

	// Get the systems timestamp
	time( &timev );

	// Convert to the local time and fill in tm obj
	timeStamp = localtime(&timev);

	// Returns pointer to struct tm in the heap
	return timeStamp;
}

/*
	Uses map to loop through FAT and find the next free block. 
	If there is a free block, the value of the FAT table for that 
	block will have the value of 9999. This will be in the FAT as
	two chars, both with the value 99 (0x63 in hex). Function will
	return value of the next free block. If there are no free blocks
	function will return -1.
*/
int getNextFreeBlock(char *map) {

	int freeBlock;		// Index of free block
	int i;				// Loop Counter

	// Loop through FAT table starting from the first one
	for(i = 0; i < SIZE_OF_FAT; i+=2) {
		if(map[i] == 99) {
			freeBlock = i;
			return freeBlock;	// Sucess
		}
	}

	return -1; 	// Free block not found
}

/*
	Changes value of the specified FAT index to newValue. It takes 
	the new value and parses them into two chars. it inserts chars
	into the FAT index and the map updates the file. If freeing blocks,
	it also frees up any blocks that it is linked too.
*/
void updateFAT(int fatIndex, char* map, int newValue) {
	int first = newValue/100;		// Lowest two digits
	int second = newValue%100;		// Highest two digits
	int inner = 0;						// Used if the link being freed has a link to another
									// 	block
	// Check if freeing block and if there is a link
	if ((map[fatIndex] != 00 && map[fatIndex + 1] != 00) && (map[fatIndex] != 99 && map[fatIndex + 1] != 99)) {
		inner = map[fatIndex] * 100;	// Get the highest sig figs of link
		inner += map[fatIndex + 1];		// Get the least sig figs of link

		updateFAT(inner, map, 9999);	// Recursively call function to free next link
	}

	// Insert chars into FAT represent free block 
	map[fatIndex] = first;					
	map[fatIndex + 1] = second;
}

/*
	This gets the FAT value of the block where the index is at. This
	index must be inside on of the data block. To get the FAT entry,
	thhe function will take the index subtract the SIZE_OF_FAT and 
	then divided the result should be the data block number that the 
	index is at. This number will tell me what FAT entry needs to be 
	returned. The function will return the two char values as a single 
	integer.
*/
int getFATLink(int index, char* map) {

	// Variables
	int FATvalue, i;

	// Error check to make the index inputed is in a data block
	if (index < SIZE_OF_FAT) {
		return -1;
	}

	// Finds which FAT entry the data blocks belong too
	index -= SIZE_OF_FAT;
	index /= SIZE_OF_BLOCKS;

	// location of the FAT table for the block
	i = index*2;

	// Converting two chars of FAT entry into single int
	FATvalue = map[i] * 100;
	FATvalue += map[i+1];

	return FATvalue;
}

/*
	Finds the index of the first character for the file header
	belonging to the file path given
*/
int findFile(char **parsePath, char* map) {

	// Variables
	int index = SIZE_OF_FAT;		// Index where the file header is
	int i=0, j=0;					// Loop Counters
	struct FileHead temp; 			// Temporay placeholder to hold values of File Header

	/* 
		Loop the parsePath, looking for each dir/file inside. If it
		finds the correct path it moves on to the next one. It does
		this until it finds the last one. It then sets index to the
		index of the data block that file header points to. 
	*/
	while (parsePath[i] != NULL && j < SIZE_OF_BLOCKS) {
		
		// Gets the file header
		temp = GetFileHead(map, index, &temp);
		
		// Checks to see if there is a match
		if (strcmp(convertFileName(parsePath[i]), temp.name) == 0) {

			// Sets index to where the file header points to 
			index = temp.blockNum*SIZE_OF_BLOCKS + SIZE_OF_FAT;

			// Increments counter to look at next item in parsePath
			i++;
		} 

		// If not found, increment counter to look at next file header in block
		else {
			index += SIZE_OF_FILEHEAD;
		}
	
	}

	// If last parsePath is not found
	if (j >= SIZE_OF_BLOCKS) {
		return -1;	// Unsucessful
	}

	// Returns index if sucessful
	else {
		return index;
	}
}

/*
	Writes header to the data block located at the index
*/
int writeHeader(struct FileHead head, char* map, int index) {

	int i=0, j=0; 			// Loop counter
	struct FileHead temp;	// Placeholder for FileHead entry
	int good = 0;			// Flag to see if the file header was found

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

	// Find empty space in data block to put header
	while (j < SIZE_OF_BLOCKS && !good) {
		
		// Pulls File header from the disk file
		temp = GetFileHead(map, index, &temp);
		
		// If the file header doesn't exist
		if (temp.type != 0) {

			// Increase counters to where the next file header is
			index += SIZE_OF_FILEHEAD;	
			j += SIZE_OF_FILEHEAD;
		
		}
		
		// File was found	
		else {
			good = 1;		// True
		}
	
	}

	// print message is no space was found
	if (!good) {
		created\n");
		return -1;
	}

	// Space was found
	else {

		// Write file header starting at the index found above
		for (i = 0; i < SIZE_OF_FILEHEAD; i++) {
			// Inserting File type
			if (i >= 0 && i <= 3) {
				map[index+i] = head.type/k;
				head.type %= k;
				k /= 10;
				ng File Name
			if (i >= 4 && i <= 15)	{
				map[index+i] = head.name[i-4];
				 Inserting File Last Access Time
			if (i >= 16 && i <= 23) {
				map[index+i] = head.lastAccess/m;
				head.lastAccess %= m;
				m /= 10;
			}

			// Inserting File Block Number
			if (i >= 24 && i <= 27) {
				map[index+i] = head.blockNum/n;
				head.blockNum %= n;
				n /= 10;
			}

			// Inserting File Size
			if (i >= 28 && i <= 31)  {
				map[index+i] = head.size/o;
				head.size %= o;
				o /= 10;
			}
		}

		return 1;
	}

}

// Functions needed to be developed
/*

	int findNextLinkBlock()
	--- Determins if there is a link to another block. returns
	--- linked block number on true or -1 if false

	struct FileHead *createFileHead(type, name, size)
	--- Creates a FileHead struct from given info. Function 
	--- automatically gets timestamp and next free block.

	int getFileSize(FILE *)
	--- Counts charaters inside file and returns the count

	int goToBlockIndex( block number )
	--- Gets the index of the specified block number
	
*/


