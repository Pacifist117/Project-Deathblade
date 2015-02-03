#include "player.h"

Player::Player(TempSettings* gamesettings) : ObjectBaseClass(gamesettings){

}

Player::~Player(){

}

void Player::step_time(){
    x += dx;
    y += dy;
}
