#include <iostream>
#include <string>
#include <boost/toxenizer.hpp>

using namespace std; //Including std this time to make code easier to read
using namespace boost;

void prompt()	{  //Add on name functionality later
	cout << "$";

}

void parseCommands(string& commandStr, vector<string>& commands)	{
	char_seperator<char> delim(";", "#", " "); //things that separate commands
	tokenizer<chaar_separator<char>> token(commandStr, delim);

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
	cout << "Work in progress \n";

}




int main(int argc, char *argv[])	{
	while(1)	{
		string input;
		vector<string> cmds;
		prompt();
		getline (cin, input);
		parseCommands(input, cmds);
		
	}

}
