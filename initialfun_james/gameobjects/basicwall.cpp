#include "gameobjects/basicwall.h"

BasicWall::BasicWall()
{
}
BasicWall::~BasicWall()
{
}

void BasicWall::create(double x, double y, double w, double h){
    this->x = x + w/2; this->y = y + h/2;
    this->w = w; this->h = h;

    bounding_points.resize(4);
    bounding_points[0].resize(2);
    bounding_points[0][0] = -w/2;
    bounding_points[0][1] = -h/2;
    bounding_points[1].resize(2);
    bounding_points[1][0] =  w/2;
    bounding_points[1][1] = -h/2;
    bounding_points[2].resize(2);
    bounding_points[2][0] =  w/2;
    bounding_points[2][1] =  h/2;
    bounding_points[3].resize(2);
    bounding_points[3][0] = -w/2;
    bounding_points[3][1] =  h/2;

}

void BasicWall::drawon(SDL_Renderer *renderer, SDL_Rect *destination){
    SDL_SetRenderDrawColor(renderer, r,g,b,a);
    SDL_RenderFillRect(renderer, destination);
}

void BasicWall::set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    r = red; g = green; b = blue; a = alpha;
}
