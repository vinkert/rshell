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

	for(tokenizer<char_separator<char>>::iterator it = token.begin(); it!= token.end(); ++it)	{ //loop fills vector with commands

	}

}






int main(int argc, char *argv[])	{
	while(1)	{
		string input;
		prompt();
		getline (cin, input);


	}

}
