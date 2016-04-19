#include "FS.h"

/*
	Creating a file/dir

	1. Make a new FileHeader
		- Find next Free Block
		- Set lastAccess
		- Set size
		- Fill in Name and Type
	
	2. Place into File Structure
		- Go to dir using parse pathname
		- Search File Header starting @ ROOT
			* If not found file is not created
		- Insert Header
		- Update FAT
*/

/*
	Open a file/dir

	1. Go to first data block of file
		- Search through dir to find the file
		- Return the index of the data block the file
*/

/* 
	Delete a file/dir

	1. Remove File header from directory structure
		- Go to the file block that contains the file header
		- Overwrite with 0's
		- Update FAT
*/

/*
	Close file

	1. Return 0
*/

/*
	Read File

	1. Return char from block at location x	

	* Like getc - reads one char at a time
*/

/*
	Write File

	1. Set char in data block at location x
	2. Fills in rest of chars after to zero

	* Like putc - writes on char at a time
*/