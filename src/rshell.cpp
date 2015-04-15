#include<iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string>
int numCommands = 0;
void prompt()	{
	std::cout << "$";
}

void parseCommands(string& commandStr, vector<string>& commands)	{
	std::vector<string> temp;
	std::string section;
	int position = 0;
	int numSections = position;

	for(std::string::iterator it = commandStr.begin(); it != commandStr.end(); ++it)	{
		if (commandStr.at(position) == ' ')	{ // checks for blank space
			if(section != " " && !section.empty())	{
				temp.push_back(section);
				numCommands++;
				section.clear();
			}
		}
		else if(commandStr.at(position) == '#')	{
			if(section != " " && !section.empty())	{
				temp.push_back(section);
			}
			temp.push_back(";");
			break;
		}
		else if ((commandstr.at(position) == ";")	{
			if(!section.empty() && section != " ")	{
				numCommands++;
				temp.push_back(section);
			}
			temp.push_back(";");
			section.clear();
		}
		else	{
			word = word + commandStr.at(numSections);
		}
		numSections++;
		position++;
	}
	if(section != " " && !section.empty())	{
		temp.pushback(section);
		numCommands++;
		if (word != ";")
			temp.pushback(";");
	}
	numCommands++;
	if (temp[temp.size() - 1) != ";")	{
		temp.push_back(";");
	}
	numCommands = temp.size();
	commands = temp;
}

void executeCommandList(vector<string>& commands)	{
	
}

int main(int argc, char **argv)	{
	while(1)	{
		prompt();
	}
	return 0;
}

