all: shell.o parse.o
	gcc -o shell shell.o parse.o

shell.o: shell.c
	gcc -c shell.c

parse.o: parse.c parse.h
	gcc -c parse.c parse.h

run:
	./shell

clean:
	rm *.o
	rm shell
