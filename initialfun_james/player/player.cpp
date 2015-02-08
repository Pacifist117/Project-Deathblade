#include "player.h"

Player::Player(){
    character_texture = NULL;
    dx = 0.03;
    dy = 0.02;
}

Player::~Player(){

}

void Player::step_time(){
    x += dx;
    y += dy;

    if (y >= 6 || y <= 0)
        dy -= 2*dy;
    if (x >= 10 || x <= 0)
        dx -= 2*dx;
}

void Player::setTexture(SDL_Texture* player_texture, double width, double height){
    character_texture = player_texture;
    w = width;
    h = height;
    std::vector<double> relpoint;
    relpoint.push_back(-w/2.0);
    relpoint.push_back(-h/2.0);
    bounding_points.push_back(relpoint);
    relpoint[0] = w/2.0;
    bounding_points.push_back(relpoint);
    relpoint[1] = h/2.0;
    bounding_points.push_back(relpoint);
    relpoint[1] = -w/2.0;
    bounding_points.push_back(relpoint);

    calculate_square_r();
}

void Player::drawon(SDL_Renderer *renderer, CameraControl *camera){

    if(character_texture == NULL) return;

    SDL_Rect dst = camera->calculate_display_destination(x-w/2.0,y-h/2.0,w,h,db::Player);
    SDL_RenderCopy(renderer, character_texture, NULL, &dst);

}
