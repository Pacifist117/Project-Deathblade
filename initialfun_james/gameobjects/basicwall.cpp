#include "gameobjects/basicwall.h"

BasicWall::BasicWall()
{
    color.r = 100;
    color.g = 100;
    color.b = 100;
    color.a = 255;
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

    calculate_square_r();

}

void BasicWall::drawon(SDL_Renderer *renderer, SDL_Rect *destination){
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer, destination);
}

void BasicWall::set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    color.r = red;
    color.g = green;
    color.b = blue;
    color.a = alpha;
}
