#include <iostream>
#include <vector>
#include <stdlib.h>
#include <errno.h>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <algorithm>

using namespace std;

void colorPrint(string& toPrint, struct stat& temp);
void printHeader(string& name, struct stat& fileName);
void printSimple(string str, struct stat& fileName);
void printWithPermissions(string str, struct stat& fileName);

bool findNext(string& name, dirent*& directoryEntries, DIR*& directory)	{
	directoryEntries = readdir(directory);
	if(errno == EBADF && directoryEntries == NULL)	{
		perror("error in rearddir");
		exit(1);
	}
	return (directoryEntries != NULL);
}

int dotChecker(string str)	{
	if(str.empty())	{
		cout << "string passed to dotChecker is null";
		exit(1);
	}
	int result = str[str.size()-1] == '.';
	result = result || ((str.size() > 1) && !str.compare(str.size() - 2, 2, "./"));
	return result;
}

void traverseDirectories(string directory, int flag)	{
	DIR* direct;
	if(directory.empty())	{
		direct = NULL;
	}
	else	{
		direct = opendir(directory.c_str());
	}
	if (direct == NULL)	{
		perror("error in opendir");
		exit(1);
	}
	unsigned i = 0;
	struct stat directoryStat;
	dirent* directoryEntries;
	vector<string> subDirectories;
	int temp = stat(directory.c_str(), &directoryStat);
	if(temp <= -1)	{
		perror("error in stat() in line 39");
		exit(1);
	}
	while(findNext(directory, directoryEntries, direct) > 0)	{
		string name = directory;
		struct stat temp;
		name += "/";
		name += directoryEntries->d_name;
		int temp1 = stat(name.c_str(), &temp);
		if(temp1 <= -1)	{
			perror("error in stat() in line 49");
			exit(1);
		}
		bool willPrint = false;
		struct stat s;
		int temp2 = stat(name.c_str(), &s);
		if(temp2 <= -1)	{
			perror("Error in stat on line 56");
			exit(1);
		}
		willPrint = ((flag&1) || directoryEntries->d_name[0] != '.');
		if(willPrint)	{
			if(flag&2)	{
				printWithPermissions(directoryEntries->d_name, s);
			}
			else	{
				printSimple(directoryEntries->d_name, s);
			}
		}
		bool descend = false;
		if(name.empty())
			cout << "empty path";
		descend = (((flag&4) != 0) || (name.find_first_of(".") == string::npos));
		descend = descend & (!dotChecker(name) && S_ISDIR(s.st_mode));
		if(descend)	{
			string copy = directory;
			copy += "/";
			copy += directoryEntries->d_name;
			subDirectories.push_back(copy);
		}
	}
	i = 0;
	while (i < subDirectories.size())	{
		string temp4 = directory;
		temp4 += ':';
		colorPrint(temp4, directoryStat);
		traverseDirectories(subDirectories[i], flag);
		i++;
	}
}

void colorPrint(string& toPrint, struct stat& temp)	{
	int compare = S_IXGRP|S_IXOTH|S_IXUSR;
	if (toPrint.at(0) == '.')	
		cout << "\x1b[;47m"; //grey
	
	if (S_ISDIR(temp.st_mode))	
		cout << "\x1b[34;1m"; //blue
	else if (compare & temp.st_mode)	
		cout << "\x1b[32;1m"; //green
	cout << toPrint << "\x1b[0m";

}

void printHeader(string& name, struct stat& fileName)	{
	cout << "\n";
	string temp = name;
	temp += ":";
	colorPrint(temp, fileName);
	cout << "\n";
}

void parseInfo(struct stat& fileName, string& grp, string& pw, string& time)	{
	group* groupID = getgrgid(fileName.st_gid);
	if (groupID == NULL)	{
		perror("error in getgrgid");
		exit(1);
	}
	grp = groupID ->gr_name;
	struct tm* time1 = localtime(&fileName.st_mtime);
	if(time1 == NULL)	{
		perror("error in localtime");
		exit(1);
	}
	time = asctime(time1);
	if (time.empty())	{
		perror("error in asctime");
		exit(1);
	}
	time = time.substr(0, time.length() - 1);
	
	passwd* userID = getpwuid(fileName.st_uid);
	if (userID == NULL)	{
		perror("error in getpwuid");
		exit(1);
	}
	pw = userID->pw_name;
}

void printWithPermissions(string str, struct stat& fileName)	{
	string grp, pw, time, permission;
	parseInfo(fileName, grp, pw, time);
	if(fileName.st_mode & S_IRWXU)	{
		permission += "r";
	}
	else	{
		permission += "-";
	}
	
	if(fileName.st_mode & S_IWUSR)	{
		permission += "w";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IXUSR)	{
		permission += "x";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IRGRP)	{
		permission += "r";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IWGRP)	{
		permission += "w";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IXGRP)	{
		permission += "x";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IROTH)	{
		permission += "r";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IWOTH)	{
		permission += "w";
	}
	else	{
		permission += "-";
	}

	if(fileName.st_mode & S_IXOTH)	{
		permission += "x";
	}
	else	{
		permission += "-";
	}
	cout << permission;
	cout << left << setw(4) << fileName.st_nlink;
	cout << left << setw(12) << pw;
	cout << left << setw(12) << grp;
	cout << left << setw(8) << fileName.st_size;
	cout << left << setw(25) << time;
	colorPrint(str, fileName);
	cout << endl; 
}

void printSimple(string str, struct stat& fileName)	{
	colorPrint(str, fileName);
	cout << "   ";
}

void printFileName(string str, struct stat& fileName, int flag)	{
	if(flag & 2)	{
		printWithPermissions(str, fileName);
	}
	else	{
		printSimple(str, fileName);
	}
}


int main(int argc, char* argv[])	{
	int whichFlag = 0;
	int i = 1;
	for(; i < argc && argv[i][0] == '-'; i++)	{
		int j = 1;
		while (argv[i][j] != '\0')	{
			char c = argv[i][j];
			if (c == 'a' || c == 'l' || c == 'R')	{
				whichFlag = (c == 'a')|whichFlag;
				whichFlag = 2*(c == 'l')|whichFlag;
				whichFlag = 4*(c == 'R')|whichFlag;	
				break;
			}
			else	{
				cerr << "Flag " << c <<  " not recognized \n";
				exit(1);
			}
			j++;
		}
	}
	vector<string> list;
	struct stat status;
	if (argc == i)
		list.push_back(".");
	while (argc > i)	{
		int temp = stat(argv[i], &status);
		if (temp <= -1)	{
			perror("error with running stat");
			exit(1);
		}
		int result = (temp != -1);
		if (result > 0)	{
			printFileName(argv[i], status, whichFlag);
			if(S_ISDIR(status.st_mode))
				list.push_back(argv[i]);
		}
		i++;
	}
	unsigned j = 0;
	while (j < list.size())	{
		struct stat status2;
		int runOk = stat(list[j].c_str(), &status2);
		if (runOk <= -1)	{
			perror("error in running stat line 281");
			exit(1);
		}
		bool checkr = !(list.size() == 1 && dotChecker(list[0]));
		checkr = checkr || whichFlag & 4;
		if(checkr)
			printHeader(list[j], status2);
		traverseDirectories(list[j], whichFlag);
		j++;
	}
	cout << "\n";
	return 0;
}
