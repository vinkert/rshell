#include <iostream>
#include <string>
#include <vector>
#include <cstdio> //perror
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/tokenizer.hpp>

using namespace std; //Including std this time to make code easier to read
using namespace boost;

void prompt()	{  //Add on name functionality later
	cout << "$";

}

void parseCommands(string& commandStr, vector<string>& commands)	{
	char_separator<char> delim(";", "#"); //things that separate commands
	tokenizer< char_separator<char> > token(commandStr, delim);

	for(auto it = token.begin(); it!= token.end(); ++it)	{ //loop fills vector with commands
		if (*it == "#")	{
			commands.push_back("#");
			break;
		}
		else	{
			commands.push_back(*it);
		}
	}
	commands.push_back("end"); //Will be used in the runCommands function to signify end of command chain
}

void runCommands(vector<string>& commands)	{ //runs the parsed vector of commands
	//remember to parse || and &&
	int numCommands = commands.size();
	vector<string> commandBlock; //Stores one instruction then gets wiped later1
	for(int i = 0; i < numCommands; ++i)	{
		if((commands.at(i).compare(";") || commands.at(i).compare("&&") || commands.at(i).compare("||")))	{

			pid_t process = fork();
				if(process == 0)	{ //Child process branch
					
				}

				else if(process <= -1)	{ //Fork failed
					perror("ERROR IN CREATING FORK");
					exit(1);
				}

				else	{ //parent process

				}
		}
	}
}




int main(int argc, char *argv[])	{
	while(1)	{
		string input;
		vector<string> cmds;
		prompt();
		getline (cin, input);
		parseCommands(input, cmds);
		for (auto it = cmds.begin(); it != cmds.end(); ++it)	{
			cout << *it << " ";

		}
		runCommands(cmds);	
	}

}
