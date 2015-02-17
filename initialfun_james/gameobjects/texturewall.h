#ifndef TEXTUREWALL_H
#define TEXTUREWALL_H

#include "gameobjects/objectbase.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

class TextureWall : public ObjectBaseClass
{
public:
    TextureWall();
    ~TextureWall();

    SDL_Texture* wall_texture;
    double width;
    double height;

    // basic private member manipulation
    void setTexture(SDL_Texture* wall_texture, double width, double height);

    void create(double x, double y, double w, double h);
    void drawon(SDL_Renderer *renderer, CameraControl* camera);


};

#endif // TEXTUREWALL_H
