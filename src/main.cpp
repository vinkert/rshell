#include <iostream>
#include <string>
#include <vector>
#include <cstdio> //perror
#include <sys/types.h>
#include <stdio.h>
#include <string.h>//strtok
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>

using namespace std;
int parseConnectors(string command, int orChecker, int andChecker);
bool execCommand(string& singleCommand);
void prompt()	{  //Add on name functionality later
	if(getlogin())
		cout << getlogin() << '@';
	char name[256] = {};
	if(gethostname(name, 255) == 0)	{
		gethostname(name, 255);
		cout << name << ' ';
	}
	cout << "$ ";

}

void parseCommands(string& commandStr)	{
	int findPound = commandStr.find("#");
	if(findPound > -1)
		commandStr.erase(findPound); //Handles comments
	char* filter = strtok(&commandStr[0], ";");
	while(filter)	{
		string temp(filter);
		parseConnectors(temp, 0, 0);
		filter = strtok(0, ";");
	}
}

int parseConnectors(string command, int orChecker, int andChecker) {  // 0 = fail, 1 = success, 2 = skip
	const char* andLocation = strstr(command.c_str(), "&&");
	const char* orLocation = strstr(command.c_str(), "||");
	if(andLocation && orLocation && orLocation < andLocation)
		andLocation = 0; 
	
	if(andLocation != 0)	{
		orChecker = parseConnectors(command.substr(0, andLocation - command.c_str()), orChecker, andChecker); //command succeeded
		
		if(orChecker == 1)
			return parseConnectors(andLocation + 2, 1, 0); 
		else	{ //Command failed
			return parseConnectors(andLocation + 2, 2, 0);
		} 
	}
	
	else if(orLocation != 0)	{ 
		andChecker = parseConnectors(command.substr(0, orLocation - command.c_str()), orChecker, andChecker);
		if(andChecker == 1)
			parseConnectors(orLocation + 2, 0, 2); 
		else	{ 
			parseConnectors(orLocation + 2, 0, 1); 
		}													}
	
	else	{ 
		if(andChecker == 2)
			return 0; 
		if(orChecker == 2)
			return 0;
		return execCommand(command);
	}
	return 0;
}

void getArgs(string& command, char* args[])	{
	vector<string> temp;
	char* removeSpaces = strtok(&command[0], " \t"); //Removes spaces and tabs
	while (removeSpaces)	{
		temp.push_back(removeSpaces);
		removeSpaces = strtok(0, " \t");
	}
	int size = temp.size();
	for(int i = 0; i < size; i++)	{
		string& swap = temp.at(i);
		args[i] = &swap[0];
	}
	args[size] = '\0';

}

bool execCommand(string& singleCommand)	{
	
	if (singleCommand == "exit")	
		exit(0);
	int process = fork();
	if (process == 0)	{
		char* args[2048];
		getArgs(singleCommand, args);

		int result = execvp(*args, args);
		if(result <= -1)
			perror("ERROR IN EXECUTING COMMAND");
	}
	else if (process <= -1)	{
		perror("ERROR IN CREATING FORK");
		return false;
	}
	else	{
		int waitChild = waitpid(process, &waitChild, 0);
		if (waitChild <= -1)	
			perror("WAITING FOR CHILD FAILED");
		if(waitChild == 0)
			return true;	
	}
	return false;
}


int main(int argc, char *argv[])	{
	while(1)	{
		string input;
		prompt();
		getline (cin, input);
		parseCommands(input);
	}

}
