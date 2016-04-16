#include "FS.h"

int main(int argc, char const *argv[])
{
	char *some = "ROOT/knrtn/djn";
	char **another;
	int i;

	parseFileName(some, another);

	for (i = 0; i < 3; ++i)
	{	
		printf("%d\n", i);
		printf("%s\n", *(another+i));
	}
	return 0;
}