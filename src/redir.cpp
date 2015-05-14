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
	if(temp.compare("exit") == 0)
		exit(1);
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
	vector<string> commandList;
	while(true)	{
		string command = prompt();
		int pid = fork();
		if(pid <= -1)	{
			perror("fork failed");
			exit(1);
		}
		if(pid == 0)	{ //Child process
			char* args[512];
			int i = 0;
			string str;
			boost::char_separator<char> separator(" ");
			boost:tokenizer <boost::char_separator<char> > tok(command, separator);
			for(boost::tokenizer <boost::char_separator<char>>::iteratotr it = tok.begin(); it != tok.end(); i++)	{
				commandList.push_back(*it);
				args[i] = new char[(*beg).size()];
				strcpy(args[i], (*beg).c_str());
				str = commandList.at(0);
				it++;
			}
			args[i] = 0;
			int j = 0;
			int check = execvp(args[0], args);
			if (check <= -1)	{
				perror("error in execvp");
				exit(1);
			}
			for(boost::tokenizer <boost::char_separator<char>>::iterator it = tok.begin(); it != tok.end(); it++)	{
				delete[] args[j];
				j++;
			}
		}
		if(pid > 0)	{
			int check2 = wait(0);
			if (check2 <= -1)	{
				perror("error in wait");
			}
		}
	}
	return 0;
}
