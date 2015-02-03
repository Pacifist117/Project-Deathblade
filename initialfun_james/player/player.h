#ifndef PLAYER_H
#define PLAYER_H

#include "gameobjects/objectbase.h"

class Player : public ObjectBaseClass {

public:
    Player(TempSettings *gamesettings);
    ~Player();


    // inherited functions
    void step_time();
};



#endif
