#include "FS.h"

int main(int argc, char const *argv[])
{

/*
*	Objectinve: Get array of strings from pathname
*/
	char path[] = "ROOT/monkey/ape";
	char str[] = "kvskjvsbjkvdsajkbjkvadskbjkjbvdsbjkvdskbj vnjsnvjn fkvwklnvnklw wnvwnoiwv wovwivwfnjio";
	
	//koCreate(2, path);
	//koDelete(path);
	printf("%d\n", koOpen(path) - SIZE_OF_FAT);
	//printf("%s\n", koRead(path));
	//koWrite(path, str);
}