all: chdelim cut grep

chdelim:
	gcc -std=c99 -o bin/chdelim src/chdelim.c src/errors.c

cut:
	gcc -std=c99 -o bin/cut src/cut.c src/errors.c

grep:
	gcc -std=c99 -o bin/grep src/grep.c src/errors.c
