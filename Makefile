all:
	mkdir -p bin
	g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell
        g++ -ansi -pedantic -Wall -Werror src/cp.cpp -o bin/cp
rshell:
	mkdir -p bin
	g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell
	g++ -ansi -pedantic -Wall -Werror src/cp.cpp -o bin/cp
