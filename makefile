all: shell.o parse_and_run.o
	gcc -o shell shell.o parse_and_run.o

shell.o: shell.c
	gcc -c shell.c

parse_and_run.o: parse_and_run.c parse_and_run.h
	gcc -c parse_and_run.c parse_and_run.h

run:
	./shell

clean:
	rm *.o
	rm shell
	rm *.gch
	rm wholist
