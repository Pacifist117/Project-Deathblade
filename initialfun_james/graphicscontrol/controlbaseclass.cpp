#include "controlbaseclass.h"

#include <sstream>
#include <iterator>

ControlBaseClass::ControlBaseClass()
{
    name = "";
}

bool ControlBaseClass::change_setting(std::string command)
{
    std::vector<std::string> arguments;
    std::stringstream ss(name+" "+command);
    std::string item;
    while (std::getline(ss,item,' '))
        arguments.push_back(item);

    return parse_arguments(arguments);
}

std::ostream* ControlBaseClass::outstream = &std::cout;
