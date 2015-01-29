#include "developer_console.h"
#include <sstream>
#include <iterator>

DeveloperConsoleClass::DeveloperConsoleClass(){
	history_maxlength = 100;
}

DeveloperConsoleClass::~DeveloperConsoleClass(){
}

void DeveloperConsoleClass::addtohistory(std::string prompt){
	if (history.size() == history_maxlength){
		history.pop_back();
	}
	history.insert(history.begin(),prompt);
}

void DeveloperConsoleClass::printandstore(std::string result){
	std::cout << result << std::endl;
	addtohistory(result);
}

void DeveloperConsoleClass::set_historymaxlength(unsigned int newlength){
	for (unsigned int i = newlength; i < history.size(); ++i)
		history.pop_back();
	history_maxlength = newlength;
}


bool DeveloperConsoleClass::execute_command(std::string command_and_arguments){

	addtohistory(command_and_arguments);

	std::istringstream iss(command_and_arguments);
	std::vector<std::string> tokens{
		std::istream_iterator<std::string>{iss}, 
		std::istream_iterator<std::string>{}
	};	

	if (tokens[0].compare("testcommand") == 0){
		printandstore("You successfully executed testcommand with several arguments: ");
		for (unsigned int i = 1; i < tokens.size(); ++i){
			printandstore(tokens[i]);
		}
		
		return true;
	}

	else if (tokens[0].compare("printhistory") == 0){
		for (int i = history.size()-1; i >= 0; --i)
			std::cout << history[i] << std::endl;
		return true;
	}

	return false;
}
