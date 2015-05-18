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

void inputRedir(const string& str);
void outputRedir(const string& str1);
void usePipe(const string& str);

void inputRedir(const string& str)	{
	char* args[2];
	string command = str;
	string file;
	boost::char_separator<char> separator(" ");
	size_t loc = command.find("<");
	if(loc <= command.size())	{
		file = command.substr(loc + 1);
		command = command.substr(0, loc);
	}
	boost::tokenizer <boost::char_separator<char> > tok1(file, separator);
	boost::tokenizer <boost::char_separator<char> >::iterator it = tok1.begin();
	int check1 = open((*it).c_str(), O_RDONLY);
	if(check1 <= -1)	{
		perror("error in open");
		exit(1);
	}
	int check2 = dup(0);
	if(check2 <= -1)	{
		perror("error in dup");
		exit(1);
	}
	int check3 = dup2(check1, 0);
	if(check3 <= -1)	{
		perror("error in dup2");
		exit(1);
	}
	close(check1);
	boost::tokenizer <boost::char_separator<char> > tok2(command, separator);
	it = tok2.begin();
	int i = 0;
	for(; it != tok2.end(); it++, i++)	{
		args[i] = new char[(*it).size()];
		strcpy(args[i], (*it).c_str());
	}
	args[i] = NULL;
	int check4 = execvp(args[0], args) == -1;
	if(check4 <= -1)	{
		perror("error in execvp");
		exit(1);
	}
	it = tok2.begin();
	int check5 = dup2(check2, 1);
	if(check5 <= -1)	{
		perror("error in dup2");
		exit(1);
	}
	close(check2);
	for(int j = 0; it != tok2.end(); j++, it++)	{
		delete[] args[j];
	}
}

void usePipe(const string& str)	{
	 char* args[2];
	 char* args2[2];
	 int fileDescriptor[2];
	 int check1 = pipe(fileDescriptor);
	 if (check1 <= -1)	{
		perror("error in pipe");
		exit(1);
	 }
	 int check2 = fork();
	 int check3;
	 if(check2 <= -1)	{
		perror("error in fork");
		exit(1);
	 }
	  else if(check2 > 0)	{
		 check3 = dup(0);
		 if(check3 <= -1)	{
			perror("error in dup");
			exit(1);
		 }
		 int check4 = wait(0);
		 if(check4 <= -1)	{
			perror("error in wait");
			exit(1);
		 }
		 int check5 = fork();
		 if(check5 <= -1)	{
			perror("error in fork");
			exit(1);
		 }
		 if(check5 > 0)	{
			int temp = wait(0);
			if(temp <= -1)	{
				perror("error in wait");
				exit(1);
			}
		 }
		 else if(check5 == 0)	{
			int check6 = dup2(fileDescriptor[0], 0);
			if(check6 <= -1)	{
				perror("error in close");
				exit(1);
			}
			int check7 = close(fileDescriptor[1]);
			if(check7 <= -1)	{
				perror("error in close");
				exit(1);
			}
			int check8 = execvp(args2[0], args2);
			if(check8 <= -1)	{
				perror("error in execvp");
				exit(1);
			}
		 }
		 else	{
			int temp2 = wait(0);
			if (temp2 <= -1)	{
				perror("error in wait");
				exit(1);
			}
		 }
	 }
	 else	{
		int check9 = close(fileDescriptor[1]);
		if(check9 <= -1)	{
			perror("error in close");
			exit(1);
		}
		int check10 = dup2(fileDescriptor[1], 1);
		if(check10 <= -1)	{
			perror("error in dup2");
			exit(1);
		}
		int check11 = close(fileDescriptor[0]);
		if(check11 <= -1)	{
			perror("error in close");
			exit(1);
		}
		int check12 = execvp(args[0], args);
		if(check12 <= -1)	{
			perror("error in execvp");
			exit(1);
		}
	 }
	 int check13 = dup2(check3, 0);
	 if(check13 <= -1)	{
		perror("error in dup2");
		exit(1);
	 }
}

void outputRedir(const string& str1)	{
	string str = str1;
	bool flag = false; //flag for appending
	char* args[2];
	string outString;
	string file;
	string command;
	int i = 0;
	boost::char_separator<char> sep(" ");
	if(str.find(">>") <= str.size())	{
		/*command = str.substr(0, str.find(">>") + 1);
		file = str.substr(str.find(">>") + 2);
		size_t temp = str.find(">>") + 1;
		if(str[temp + 1] == '>')	{
			flag = true;
			str[temp + 1] = ' ';			
		}*/
		str.erase(str.find(">>"), 1);
	}
	if(str.find(">") <= str.size())	{
		command = str.substr(0, str.find(">"));
		file = str.substr(str.find(">") + 1);
		size_t temp = str.find(">");
		if(str[temp + 1] == '>')	{
			flag = true;
			str[temp + 1] = ' ';
		}
	}
	if(flag)	{
		boost::tokenizer <boost::char_separator<char> > tok(file, sep);
		boost::tokenizer <boost::char_separator<char> >::iterator it = tok.begin();
		int check1 = open((*it).c_str(), O_APPEND | O_CREAT | O_WRONLY, 0666);
		if(check1 <= -1)	{
			perror("error in open");
			exit(1);	
		}
		int check2 = dup(1);
		if(check2 <= -1)	{
			perror("error in dup");
			exit(1);
		}
		int check3 = dup2(check1, 1);
		if(check3 <= -1)	{
			perror("error in dup2");
			exit(1);
		}
		close(check1);
		boost::tokenizer <boost::char_separator<char> > tok2(command, sep);
		int j = 0;
		for (boost::tokenizer <boost::char_separator<char> >::iterator it = tok2.begin(); it != tok2.end(); it++)	{
			
			args[j] = new char[(*it).size()];
			strcpy(args[j], (*it).c_str());			
			j++;	
		}
		args[j] = NULL;
		int check4 = execvp(args[0], args);
		if(check4 <= -1)	{
			perror("error in execvp");
			exit(1);
		}
		int i = 0;
		for(boost::tokenizer <boost::char_separator<char> >::iterator it = tok2.begin(); it != tok2.end(); it++)	{
			delete[] args[i];
			i++;
		}
 		int check5 = dup2(check2, 1);
		if (check5 <= -1)	{
			perror("error in dup2");
			exit(1);
		}
		close(check2);
		flag = false;
	}

	else	{
		boost::tokenizer <boost::char_separator<char> > tok(file, sep);
		boost::tokenizer <boost::char_separator<char> >::iterator it = tok.begin();
		int check1 = open((*it).c_str(), O_TRUNC | O_CREAT | O_WRONLY, 0666);
		if(check1 <= -1)	{
			perror("error in open");
			exit(1);	
		}
		int check2 = dup(1);
		if(check2 <= -1)	{
			perror("error in dup");
			exit(1);
		}
		int check3 = dup2(check1, 1);
		if(check3 <= -1)	{
			perror("error in dup2");
			exit(1);
		}
		close(check1);
		boost::tokenizer <boost::char_separator<char> > tok2(command, sep);
		int j = 0;
		for (boost::tokenizer <boost::char_separator<char> >::iterator it = tok2.begin(); it != tok2.end(); it++)	{
			args[j] = new char[(*it).size()];
			strcpy(args[j], (*it).c_str());			
			j++;
		}
		args[j] = NULL;
		int check4 = execvp(args[0], args);
		if(check4 <= -1)	{
			perror("error in execvp");
			exit(1);
		}
		int i = 0;
		for(boost::tokenizer <boost::char_separator<char> >::iterator it = tok2.begin(); it != tok2.end(); it++)	{

			delete[] args[i];
			i++;
		}
 		int check5 = dup2(check2, 1);
		if (check5 <= -1)	{
			perror("error in dup2");
			exit(1);
		}
		close(check2);
		
	}
}

string prompt()	{
	cout << "$: ";
	string temp;
	getline(cin, temp);
	size_t length = temp.size();
	if(temp.compare("exit") == 0)
		exit(1);
	int comment = temp.find("#");
	if(comment <= length)
		temp = temp.substr(0, comment);
	size_t location = temp.find(">>");
	if(location <= length)	{
		outputRedir(temp);
	}
	else	{
		location = temp.find(">");
		if(location <= length)	{
			outputRedir(temp);
		}
	}
	location = temp.find("<");
	if(location <= length)	{
		inputRedir(temp);
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
	//	cout << "COMMAND IS " << command << endl;

		int pid = fork();
		if(pid <= -1)	{
			perror("fork failed");
			exit(1);
		}
		if(pid == 0)	{ //Child process
			char* argv[512];
			int i = 0;
			string str;
			boost::char_separator<char> separator(" ");
			boost:tokenizer <boost::char_separator<char> > tok(command, separator);
			for(boost::tokenizer <boost::char_separator<char> >::iterator it = tok.begin(); it != tok.end(); i++)	{
			
				commandList.push_back(*it);
				argv[i] = new char[(*it).size()];
				strcpy(argv[i], (*it).c_str());
				str = commandList.at(0);
				it++;
				
			}
			//argv[i] = 0;
			//cout << "ELEMENT1 IS " << argv[0] << endl;
			//cout << "ELEMENT2 IS " << argv[1] << endl;
		//for(int k = 0; k <= i;k++)	{
			//	cout << "ELEMENT K IS " << argv[k] << endl;

			//}
			int j = 0;
			int check = execvp(argv[0], argv);
			if (check <= -1)	{
				perror("error in execvp");
				exit(1);
			}
			for(boost::tokenizer <boost::char_separator<char> >::iterator it = tok.begin(); it != tok.end(); it++)	{
				delete[] argv[j];
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
