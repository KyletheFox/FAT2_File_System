#include "FS.h"

int main(int argc, char const *argv[]) {
	
	int i;
	int fd;
	char *map;
	struct stat fileInfo;

	// Opens File
	fd = open("FS.txt", O_RDONLY);

	// Gets file info and place into struct
	printf("%d\n", fstat(fd, &fileInfo));
	printf("%d\n", fileInfo.st_size);
	
	//map = mmap(NULL, );

	close(fd);

	return 0;
}