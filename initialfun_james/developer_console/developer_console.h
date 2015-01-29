#include <iostream>
#include <vector>
#include <string>

class DeveloperConsoleClass {
	
	private:
		std::vector<std::string> history;
		unsigned int history_maxlength;
		unsigned int history_beginning;
		void addtohistory(std::string prompt);
		void printandstore(std::string result);
			
	public:
		DeveloperConsoleClass();
		~DeveloperConsoleClass();

		void set_historymaxlength(unsigned int newlength);
		
		bool execute_command(std::string command_and_arguments);

};
