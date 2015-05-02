#include <iostream>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "Timer.h"
using namespace std;

void cPPWrite(char* input, char* output)	{
	Timer time;
	double wallClock;
	double usrTime;
	double sysTime;
	time.start();
	ifstream in(input);
	ofstream out(output);
	char c;
	while(in.get(c))
		out.put(c);
	in.close();
	out.close();
	time.elapsedWallclockTime(wallClock);
	time.elapsedUserTime(usrTime);
	time.elapsedSystemTime(sysTime);
	cout << "Wallclock Time: " << wallClock << endl;
	cout << "User Time: " << usrTime << endl;
	cout << "System Time: " << sysTime << endl;
}

void cpByCharacter(char* input, char* output)	{
	Timer time;
	double wallClock;
	double usrTime;
	double sysTime;
	time.start();
	int fout;
	int fin;
	int character;
	char buffer[1];
	fin = open(input, O_RDONLY);
	if (fin == -1)	{
		perror("Error in opening input file");
		exit(1);	
	}
	fout = open(output, O_RDWR | O_CREAT, 0644);
	if (fout == -1)	{
		perror("Error in creating output file");
		exit(1);
	}
	while((character = read(fin, buffer, 1)))	{
		if (character == -1)	{
			perror("Error in reading input file");
			exit(1);
		}
		character = write(fout, buffer, 1);
		if (character == -1)	{
			perror("Error in writing to output file");
			exit(1);
		}
	}
	fin = close(fin);
	if (fin == -1)	{
		perror("Error in closing input file stream");
		exit(1);
	}
	fout = close(fout);
	if (fout == -1)	{
		perror("Error in closing output file stream");
		exit(1);
	}
	time.elapsedWallclockTime(wallClock);
	time.elapsedUserTime(usrTime);
	time.elapsedSystemTime(sysTime);
	cout << "Wallclock Time: " << wallClock << endl;
	cout << "User Time: " << usrTime << endl;
	cout << "System Time: " << sysTime << endl;
}

void cpByBuffer(char* input, char* output)	{
	Timer time;
	double wallClock;
	double usrTime;
	double sysTime;
	time.start();
	char buffer[BUFSIZ];
	int fout;
	int fin;
	int character;
	fin = open(input, O_RDONLY);
	if (fin == -1)	{
		perror("Error in opening input file");
		exit(1);	
	}
	fout = open(output, O_RDWR | O_CREAT, 0644);
	if (fout == -1)	{
		perror("Error in creating output file");
		exit(1);
	}
	while((character = read(fin, buffer, BUFSIZ)))	{
		if (character == -1)	{
			perror("Error in reading input file");
			exit(1);
		}
		character = write(fout, buffer, BUFSIZ);
		if (character == -1)	{
			perror("Error in writing to output file");
			exit(1);
		}
	}
	fin = close(fin);
	if (fin == -1)	{
		perror("Error in closing input file stream");
		exit(1);
	}
	fout = close(fout);
	if (fout == -1)	{
		perror("Error in closing output file stream");
		exit(1);
	}
	time.elapsedWallclockTime(wallClock);
	time.elapsedUserTime(usrTime);
	time.elapsedSystemTime(sysTime);
	cout << "Wallclock Time: " << wallClock << endl;
	cout << "User Time: " << usrTime << endl;
	cout << "System Time: " << sysTime << endl;
}

bool fileExists(char* fileName)	{
	ifstream in(fileName);
	return in.good();
}

int main(int argc, char *argv[])	{
	char* inputFile = argv[1];
	char* outputFile = argv[2];
	if (fileExists(outputFile))	{
		perror("Error - output file already exists");
		exit(1);
	}
	if (argc == 4)	{
		cout << "EXECUTING COPY BY CHARACTER\n";
		cpByCharacter(inputFile, outputFile);
		cout << "EXECUTING COPY BY BUFFER\n";
		cpByBuffer(inputFile, outputFile);
		cout << "EXECUTING COPY USING C++\n";
		cPPWrite(inputFile, outputFile);
	}
	else	{
		cPPWrite(inputFile, outputFile);
	}
	return 0;
}

