#include "player.h"

Player::Player(){
    character_texture = NULL;
    texturewidth = 0;
    textureheight = 0;
    x = 5;
    y = 0.1;
    dx = 0.03;
    dy = 0.0;
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
    texturewidth = height;

    bounding_points.clear();
    bounding_points.push_back(vec2d(x-width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y-height/2.0));
    bounding_points.push_back(vec2d(x+width/2.0,y+height/2.0));
    bounding_points.push_back(vec2d(x-width/2.0,y+height/2.0));

    organize_points();

}

void Player::drawon(SDL_Renderer *renderer, CameraControl *camera){

    if(character_texture == NULL) return;

    SDL_Rect dst = camera->calculate_display_destination(x-0.05/2.0,y-0.05/2.0,0.05,0.05,db::Player);
    SDL_RenderCopy(renderer, character_texture, NULL, &dst);

}
