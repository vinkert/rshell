all:
	mkdir -p bin
	#g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell
	#g++ -ansi -pedantic -Wall -Werror src/redir.cpp -o bin/rshell
	g++ -ansi -pedantic -Wall -Werror src/cd.cpp -o bin/rshell
	g++ -ansi -pedantic -Wall -Werror src/cp.cpp -o bin/cp
	g++ -ansi -pedantic -Wall -Werror src/ls.cpp -o bin/ls
rshell:
	mkdir -p bin
	#g++ -ansi -pedantic -Wall -Werror src/redir.cpp -o bin/rshell
	#g++ -ansi -pedantic -Wall -Werror src/main.cpp -o bin/rshell
	g++ -ansi -pedantic -Wall -Werror src/cd.cpp -o bin/rshell
	g++ -ansi -pedantic -Wall -Werror src/cp.cpp -o bin/cp
	g++ -ansi -pedantic -Wall -Werror src/ls.cpp -o bin/ls
