#include "objectbase.h"

ObjectBaseClass::ObjectBaseClass(){

    x = 0; y = 0; zplane = db::Player;
    w = 0; h = 0; dx = 0; dy = 0;

}

ObjectBaseClass::~ObjectBaseClass(){

}

void ObjectBaseClass::step_time(){
    x += dx;
    y += dy;
}


void ObjectBaseClass::drawon(SDL_Renderer *renderer, SDL_Rect *destination){

}

void ObjectBaseClass::bounceoff_static(ObjectBaseClass* staticobject){

}
