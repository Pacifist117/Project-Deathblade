#include <iostream>
#include "developer_console.h"

int main(){

    DeveloperConsoleClass theconsole(NULL);

    std::cout << "\n=== Developer Console Tester ===\n";
    std::string input = "";
    while(input.compare("quit") != 0){
        std::cin >> input;
        theconsole.execute_command(input);
    }

    return 0;
}
