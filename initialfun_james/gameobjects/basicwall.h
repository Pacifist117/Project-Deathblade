#ifndef BASICWALL_H
#define BASICWALL_H

#include "gameobjects/objectbase.h"
#include "SDL.h"

class BasicWall : public ObjectBaseClass
{
public:
    BasicWall();
    ~BasicWall();

    void create(double x, double y, double w, double h);
    void drawon(SDL_Renderer *renderer, SDL_Rect *destination);

    void set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

private:
    Uint8 r,g,b,a;
};

#endif // BASICWALL_H
