#ifndef BASICWALL_H
#define BASICWALL_H

#include "gameobjects/objectbase.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

class BasicWall : public ObjectBaseClass
{
public:
    BasicWall();
    ~BasicWall();

    void create(double x, double y, double w, double h);
    void drawon(SDL_Renderer *renderer, CameraControl* camera);

    void set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

private:
    SDL_Color color;
};

#endif // BASICWALL_H
