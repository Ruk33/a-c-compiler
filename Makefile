all:
	gcc -std=c11 -I. main.c lib/hashmap.c
	mv a.exe bin/vrjc.exe
