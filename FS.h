#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct FATentry{
	char next;
	char block;
} FATentry;

#define NUM_OF_BLOCKS 4
#define SIZE_OF_BLOCKS 10
#define SIZE_OF_FAT NUM_OF_BLOCKS * sizeof(struct FATentry)