clean:
	rm -rf maze maze-debug* maze*.tar.bz2

build:
	gcc -Wall -o maze maze.c

build-debug:
	gcc -Wall -g -o maze-debug maze.c

bz2:
	tar -cjf maze-source.tar.bz2 maze.c maze.h Makefile

bz2-build: build
	tar -cjf maze.tar.bz2 maze

bz2-debug: build-debug
	tar -cjf maze-debug.tar.bz2 maze-debug
