create: CreateDisk.c FSFunctions.c
	g++ -Wall -o createDisk FSFunctions.c CreateDisk.c -I.

display: DisplayDisk.c FSFunctions.c
	g++ -Wall -o displayDisk FSFunctions.c DisplayDisk.c -I.

printB: printBytes.c 
	g++ -Wall - prtB printBytes.c -I.

some: some.c FSFunctions.c diskFunc.c
	g++ -Wall -o play some.c FSFunctions.c diskFunc.c -I.

