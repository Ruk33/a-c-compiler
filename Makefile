all:
	gcc -std=c11 -I. main.c lib/hashmap.c src/helpers/list.c
	mkdir -p bin
	mv a.exe bin/vrjc.exe
