#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

std_out(const string& str);
std_in(const string& str);
usePipe(const string& str);

string prompt()	{
	cout << "$: ";
	string temp;
	size_t length = temp.size()
	cin >> temp;
	int comment = temp.find("#");
	if(comment <= length)
		temp = temp.substr(0, comment);
	size_t location = temp.find(">");
	if(location <= length)	{
		std_out(temp);
	}
	location = temp.find("<");
	if(location <= length)	{
		std_in(temp);
	}
	location = temp.find("|");
	if(location <= length)	{
		usePipe(temp);
	}
	return temp;
}

int main(int argc, char** argv)	{
	while(true)	{
		string command = prompt();
		
	}
}
