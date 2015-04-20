all:
	mkdir -p bin
	g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell

rshell:
	mkdir -p bin
	g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell
