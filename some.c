#include "FS.h"

int main(int argc, char const *argv[])
{

/*
*	Objectinve: Get array of strings from pathname
*/
	char path[] = "ROOT/";
	
	//koCreate(2, path);
	//koDelete(path);
	//printf("%d\n", koOpen(path) - SIZE_OF_FAT);
	koRead(path);
}