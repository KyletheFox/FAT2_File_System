create: CreateDisk.c FSFunctions.c
	gcc -Wall -o createDisk FSFunctions.c CreateDisk.c -I.

display: DisplayDisk.c FSFunctions.c
	gcc -o displayDisk FSFunctions.c DisplayDisk.c -I.
