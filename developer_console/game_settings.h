#ifndef GAME_SETTIGNS_H
#define GAME_SETTINGS_H

#include <string>

namespace deathblade{


template<class ValType> class Setting {
public:
    std::string name;
    std::string description;
    ValType value;
};


class SettingsBaseClass {
public:

    template<class ValType> ValType getSetting(std::string setting_name);
};


#endif

} // deathblade namespace
