all: chdelim cut

chdelim:
	gcc -std=c99 -o bin/chdelim src/chdelim.c src/errors.h src/errors.c

cut:
	gcc -std=c99 -o bin/cut src/cut.c src/errors.h src/errors.c
