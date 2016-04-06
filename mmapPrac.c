#include "FS.h"

int main(int argc, char const *argv[]) {
	
	int i;
	int fd;
	char *map;
	struct stat fileInfo;

	// Opens File
	fd = open("LICENSE", O_RDONLY);

	// Gets file info and place into struct
	fstat(fd, &fileInfo);
	printf("%d\n", fileInfo.st_size);
	
	map = mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);

	for (i = 0; i < fileInfo.st_size; ++i)	{
		printf("Entry: %d, Value: %c\n", i, map[i]);
	}

	munmap(map, fileInfo.st_size);

	close(fd);

	return 0;
}