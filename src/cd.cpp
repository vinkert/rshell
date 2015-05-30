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
	char path[1024];
	char* check = getcwd(path, 1023);
	if(check == NULL)	{
		perror("error in getcwd");
	}
	cout << path;
	cout << "$ ";

}

void parseCommands(string& commandStr)	{
	int findPound = commandStr.find("#");
	if(findPound > -1)
		commandStr.erase(findPound); //Handles comments
	char* filter = strtok(&commandStr[0], ";");
	while(filter)	{
		string temp(filter);
		if(temp.find("exit") != string::npos)
			exit(0);
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

void getArgs1(string& command, char* args)	{
	vector<string> temp;
	char* removeSpaces = strtok(&command[0], " \t"); //Removes spaces and tabs
	while (removeSpaces)	{
		temp.push_back(removeSpaces);
		removeSpaces = strtok(0, " \t");
	}
	int size = temp.size();
	for(int i = 0; i < size; i++)	{
		string swap = temp.at(i);
		args[i] = swap[0];
	}
	args[size] = '\0';

}


void handle_SIGINT(int signal)	{
//	cout << endl;
}

/*string getPath(string str)	{
	queue <string> pathQueue;
	const char* path = getenv("PATH");
	if(path == NULL)	
		perror("error in getenv");
	string result = path;
	boost::char_separator<char> separatorSymbol(":");
	boost::tokenizer <boost::char_separator<char> > sep(result, separatorSymbol);
	for(boost::tokenizer <boost::char_separator<char> >::iterator it = sep.begin(); it != sep.end(); it++)	{
		pathQueue.push(*it + "/" + str);
	}
	int sz = pathQueue.size();
	for(unsigned i = 0; i < sz; i++)	{
		struct stat buffer;
		string temp = pathQueue.front();
		pathQueue.pop();
		if(stat(check.c_str(), &buffer) == -1)	{
			continue;
			perror("Error in stat");
		}
		else	{
			return temp;
		}
	}
	return str;
}*/

bool execCommand(string& singleCommand)	{
	if (singleCommand.compare("exit") == 0)	
		exit(0);
	//if(singleCommand.compare("cd") == 0)	{
	int loc = singleCommand.find("cd");
	if(loc > -1 )	{
		/*const char* path = getenv("HOME");
		chdir(path);
		string input;
		getline (cin, input);
		parseCommands(input);
		return true;*/
		string home;
		string dir;
		char* str;
		char path[2048];
		//char* arguments;
		const char* arguments;
		string temp = "";
		if (singleCommand.size() > 2)	
			temp = singleCommand.substr(loc + 3);
		if (singleCommand.size() > 2)
			arguments = temp.c_str();
		if(temp.size() == 0)	{
			home = "";
			char* dir = getenv("HOME");
			if(dir == NULL)	
				perror("error in getenv");
			else	{
				//home = home + str;
				home = home + dir;
			}
			char* check2 = (getcwd(path, 2047));
			if(check2 == '\0')	{
				perror("error in get cwd");
			}
			else {
				int check3 = setenv("OLDPWD", path, 1);
				if (check3 <= -1)	{
					perror("error in setenv");
				}
			}		
			int check1 = chdir(home.c_str());
			if(check1 <= -1)	{
				perror("error in chdir");
			}
			return true;
		}
		else if(strcmp(arguments, "-") == 0)	{
			dir = "";
			str = getenv("OLDPWD");
			if(str == '\0')
				perror("error in getenv");
			else
				dir += str;
				char* check2 = (getcwd(path, 2047));
			if(check2 == '\0')	{
				perror("error in get cwd");
			}
			else {
				int check3 = setenv("OLDPWD", path, 1);
				if (check3 <= -1)	{
					perror("error in setenv");
				}
			}		
			int check1 = chdir(dir.c_str());
			if(check1 <= -1)	{
				perror("error in chdir");
			}
			return true;
		
		}

		else if((strcmp(arguments, "~") == 0))	{
			home = "";
			char* dir = getenv("HOME");
			if(dir == NULL)	
				perror("error in getenv");
			else	{
				//home = home + str;
				home = home + dir;
			}
			char* check2 = (getcwd(path, 2047));
			if(check2 == '\0')	{
				perror("error in get cwd");
			}
			else {
				int check3 = setenv("OLDPWD", path, 1);
				if (check3 <= -1)	{
					perror("error in setenv");
				}
			}		
			int check1 = chdir(home.c_str());
			if(check1 <= -1)	{
				perror("error in chdir");
			}
			return true;

			/*home = "";

			char* dir = getenv("HOME");
			if(dir == NULL)	
				perror("error in getenv");
			else	{
				home = home + str;
			}
			char* check2 = (getcwd(path, 2047));
			if(check2 == '\0')	{
				perror("error in get cwd");
			}
			else {
				int check3 = setenv("OLDPWD", path, 1);
				if (check3 <= -1)	{
					perror("error in setenv");
				}
			}		
			int check1 = chdir(home.c_str());
			if(check1 <= -1)	{
				perror("error in chdir");
			}
			return true;*/
	
		}
		char* check2 = (getcwd(path, 2047));
		if(check2 == '\0')	{
			perror("error in get cwd");
		}
		else {
			int check3 = setenv("OLDPWD", path, 1);
			if (check3 <= -1)	{
				perror("error in setenv");
			}
		}
		int check1 = chdir(arguments);
		if(check1 <= -1)	{
			perror("error in chdir");
		}
		
		return true;
	}
	int process = fork();
	if (process <= -1)	{
		perror("ERROR IN CREATING FORK");
		return false;
	}
	if(signal(SIGINT, handle_SIGINT) == SIG_ERR)	
		perror ("ERROR IN SIGNAL");
	else if (process == 0)	{
		char* args[2048];
		getArgs(singleCommand, args);
		
		int result = execvp(*args, args);
		if(result <= -1)	{
			perror("ERROR IN EXECUTING COMMAND LINE 202");
			exit(1);
		}
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
		if(signal(SIGINT, handle_SIGINT) == SIG_ERR)	
			perror ("ERROR IN SIGNAL");
		getline(cin, input);
		int loc = input.find("exit");
		if(loc > -1)
			exit(0);
		parseCommands(input);
	}

}
