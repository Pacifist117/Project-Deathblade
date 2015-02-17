#ifndef PLAYER_H
#define PLAYER_H

#include "gameobjects/objectbase.h"
#include "player/boundclass.h"

class Player : public ObjectBaseClass {

public:
    Player();
    ~Player();

    double xinput, yinput;
    SDL_Texture* character_texture;
    double texturewidth;
    double textureheight;

    BoundClass bound;
    void rebound();

    // basic private member manipulation
    void setTexture(SDL_Texture* player_texture, double width, double height);

    // inherited functions
    void step_time();
    bool isMobile(){return true;}
    bool isPlayer(){return true;}
    void drawon(SDL_Renderer* renderer, CameraControl* camera);
};



#endif
