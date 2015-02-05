#include "player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::step_time(){
    x += dx;
    y += dy;
}
