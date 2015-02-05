#ifndef PLAYER_H
#define PLAYER_H

#include "gameobjects/objectbase.h"

class Player : public ObjectBaseClass {

public:
    Player();
    ~Player();

    double xinput, yinput;

    // inherited functions
    void step_time();
    bool isMobile(){return true;}
    //bool isPoints(){return true;} // real thing
};



#endif
