#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include "Timer.h"

using namespace std;

int cplusplusMeth(char *argv[]) {
	ifstream is(argv[1]);
	ofstream ou(argv[2]);

	char c;
	while (is.get(c)) {
		ou.put(c);
	}
	is.close();
	return 0;
}

int unix1char(char *argv[]) {
	char buf[1];
	int input_fd, output_fd, r, w;

	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1) {
		perror("open");
		return -1;
	}

	output_fd = open(argv[2], O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);
	if (output_fd == -1) {
		perror("open");
		return -1;
	}

	while ((r = read(input_fd, &buf, 1)) > 0) {
		w = write(output_fd, &buf, r);
		if (w != r) {
			perror("write");
			return -1;
		}
	}
	return 0;
}

int unix1buff(char*argv[]) {
	char buf[BUFSIZ];
	int input_fd, output_fd, r, w;

	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1) {
		perror("open");
		return -1;
	}

	output_fd = open(argv[2], O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);
	if (output_fd == -1) {
		perror("open");
		return -1;
	}

	while ((r = read(input_fd, &buf, BUFSIZ)) > 0) {
		w = write(output_fd, &buf, r);
		if (w != r) {
			perror("write");
			return -1;
		}
	}
	return 0;
}
bool fileExists(char ch[]) {
	ifstream in(ch);
	return in.good();
}


int main(int argc, char *argv[]) {
 	Timer t;
	double eTime[10];
	//t.start();	
	//t.elapsedUserTime(eTime);
	//eTime is then holding the time

	//check if 1st argument is directory, print error message if true
	if (fileExists(argv[2])) {
		perror("Output file already exists");
		return 1;
	}
	//if argc < 4, do optimal meth
	if(argc < 4)
		cplusplusMeth(argv);
	else {
		t.start();
		unix1buff(argv);
		t.elapsedUserTime(eTime[0]);
		t.elapsedSystemTime(eTime[1]);
		t.elapsedWallclockTime(eTime[2]);

		t.start();
		unix1char(argv);
		t.elapsedUserTime(eTime[3]);
		t.elapsedSystemTime(eTime[4]);
		t.elapsedWallclockTime(eTime[5]);


		t.start();
		cplusplusMeth(argv);
		t.elapsedUserTime(eTime[6]);
		t.elapsedSystemTime(eTime[7]);
		t.elapsedWallclockTime(eTime[8]);

		cout << "C++ method runtimes: " << endl;
		cout << "    Wallclock: " << eTime[2] << endl;
		cout << "    User: " << eTime[0] << endl;
		cout << "    System: " << eTime[1] << endl;

		cout << "Unix syscalls method, one char runtimes: " << endl;
		cout << "    Wallclock: " << eTime[5] << endl;
		cout << "    User: " << eTime[3] << endl;
		cout << "    System: " << eTime[4] << endl;

		cout << "Unix sysclass method, one buffer runtimes: " << endl;
		cout << "    Wallclock: " << eTime[8] << endl;
		cout << "    User: " << eTime[6] << endl;
		cout << "    System: " << eTime[7] << endl;
	}

	return 0;
}
