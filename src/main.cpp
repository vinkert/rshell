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

void parseCommands(string& commandStr)	{
	char* filter = strtok(&commandStr[0], ";");
	while(filter)	{
		parseConnectors(filter, 0, 0);
	}
}

int parseConnectors(string& command, int orChecker, int andChecker) {  // 0 = fail, 1 = success, 2 = skip
	const char* andLocation = strstr(command.c_str(), "&&");
	const char* orLocation = strstr(command.c_str(), "||");
	if(andLocation && orLocation && orLocation < andLocation)
		andLocation = 0; 
	
	if(andLocation !- 0)	{
		orChecker = parseConnectors(input.substr(0, andLocation - input.c_str()), crChecker, andChecker); //command succeeded
		if(orChecker == 1)
			return parseConnectors(andLocation + 2, 1, 0); 
		else	{ //Command failed
			return parseConnectors(andLocation + 2, 2, 0);
		} 
	}
	
	else if(orLocation != 0)	{ 
		andChecker = parseConnectors(input.substr(0, orLocation - input.c_str()), orChecker, andChecker);
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


bool execCommand(string& input)	{
	
	if (input == "exit")	
		exit(0);
	int process = fork();
	if (process == 0)	{
		char* args[2048];
		vector<string> arg;

	}
}

void runCommands(vector<string>& commands)	{ //runs the parsed vector of commands
	//remember to parse || and &&
	int execvpStatus = 0;
	int numCommands = commands.size();
	char** commandBlock = new char*[numCommands]; //c-string of commands
	for(int i = 0; i < numCommands; ++i)	{
		if((commands.at(i).compare(";") || commands.at(i).compare("&&") || commands.at(i).compare("||")))	{

			int process = fork();
				if(process == 0)	{ //Child process branch
					//commandBlock[loc] = NULL;
					execvpStatus = execvp(commandBlock[0], commandBlock);					
					if(execvpStatus <= -1)	{
						perror("ERROR IN EXECUTING COMMAND");
						int j = i;
						bool foundConnecter = false;
						if (commands.at(j).compare("&&"))	
							j++;
						for (;j < numCommands; j++)	{
							if (commands
						}

					}
				}

				else if(process <= -1)	{ //Fork failed
					perror("ERROR IN CREATING FORK");
					exit(1);
				}

				else	{ //parent process
					int waitStatus = waitpid(process, &execvpStatus, 0);
					if (waitStatus <= -1)	
						perror("ERROR IN WAIT");
					
				}
		}
	}
}



int main(int argc, char *argv[])	{
//	while(1)	{
		string input;
		vector<string> cmds;
		prompt();
		getline (cin, input);
		parseCommands(input, cmds);
		for (auto it = cmds.begin(); it != cmds.end(); ++it)	{
			cout << *it << "\n";
		}
		cout << "PARSED COMMANDS SIZE: " << cmds.size() << endl;
		runCommands(cmds);	
//	}

}
