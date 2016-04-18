#include "FS.h"

int main(int argc, char const *argv[])
{
/*
*	Objectinve: U7se getTimeStamp() - Working
*/
	struct tm *stamp;
	stamp = getTimeStamp();
	printf("%d\n", stamp->tm_mon);
	printf("%d\n", 1900 + stamp->tm_year);
	printf("%d\n", stamp->tm_mday);
	return 0;
	
}