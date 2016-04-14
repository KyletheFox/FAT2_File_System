create: CreateDisk.c FSFunctions.c
	gcc -o createDisk FSFunctions.c CreateDisk.c -I.

display: displayDisk.c FSFunctions.c
	gcc -o displayDisk FSFunctions.c displayDisk.c -I.
