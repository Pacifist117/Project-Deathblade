#ifndef PLAYER_H
#define PLAYER_H

#include "gameobjects/objectbase.h"

class Player : public ObjectBaseClass {

public:
    Player();
    ~Player();

    double xinput, yinput;
    SDL_Texture* character_texture;

    // basic private member manipulation
    void setTexture(SDL_Texture* player_texture, double width, double height);

    // inherited functions
    void step_time();
    bool isMobile(){return true;}
    void drawon(SDL_Renderer* renderer, CameraControl* camera);
};



#endif
