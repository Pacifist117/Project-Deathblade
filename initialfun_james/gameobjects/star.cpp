#include "gameobjects/star.h"

Star::Star(){
    star_texture = NULL;
    texturewidth = 0;
    textureheight = 0;
}

Star::~Star(){

}

void Star::setTexture(SDL_Texture* star_texture, double width, double height){
    this->star_texture = star_texture;
    texturewidth = width;
    textureheight = height;

    bounding_points.clear();
    bounding_points.push_back(vec2d(x-width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y+height/2.0));
    bounding_points.push_back(vec2d(x-width/2.0,y+height/2.0));

    organize_points();
}


void Star::drawon(SDL_Renderer *renderer, CameraControl *camera){

    if(star_texture == NULL) return;

    SDL_Rect dst = camera->calculate_display_destination(
                x-texturewidth/2.0,
                y-textureheight/2.0,
                texturewidth,
                textureheight,
                db::Player);

    SDL_RenderCopyEx(renderer, star_texture, NULL, &dst, th*180.0/3.14159265359, NULL, SDL_FLIP_NONE);

}
