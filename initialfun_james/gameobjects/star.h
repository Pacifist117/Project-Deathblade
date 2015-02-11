#ifndef STAR_H
#define STAR_H

#include "gameobjects/objectbase.h"
#include "SDL.h"

class Star : public ObjectBaseClass
{
public:
    Star();
    ~Star();


    SDL_Texture* star_texture;
    double texturewidth;
    double textureheight;

    // basic private member manipulation
    void setTexture(SDL_Texture* player_texture, double width, double height);

    // inherited functions
    void step_time(){}
    bool isMobile(){return false;}
    void drawon(SDL_Renderer* renderer, CameraControl* camera);
};

#endif // STAR_H
