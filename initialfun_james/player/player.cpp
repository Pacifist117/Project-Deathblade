#include "player.h"

Player::Player(){
    character_texture = NULL;
    texturewidth = 0;
    textureheight = 0;
    mass = 150;
}

Player::~Player(){

}

void Player::step_time(){
    translate(dx,dy);
    rotate(dth);
}

void Player::setTexture(SDL_Texture* player_texture, double width, double height){
    character_texture = player_texture;
    texturewidth = width;
    textureheight = height;

    bounding_points.clear();
    bounding_points.push_back(vec2d(x-width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y+height/2.0));
    bounding_points.push_back(vec2d(x-width/2.0,y+height/2.0));

    organize_points();

}

void Player::drawon(SDL_Renderer *renderer, CameraControl *camera){

    if(character_texture == NULL) return;

    SDL_Rect dst = camera->calculate_display_destination(
                x-texturewidth/2.0,
                y-textureheight/2.0,
                texturewidth,
                textureheight,
                db::Player);

    SDL_Point center;
    center.x = camera->pixelfromx(x,db::Player);
    center.y = camera->pixelfromy(y,db::Player);
    SDL_RenderCopyEx(renderer, character_texture, NULL, &dst, th, &center, SDL_FLIP_NONE);

}
