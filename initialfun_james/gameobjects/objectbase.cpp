#include "objectbase.h"

ObjectBaseClass::ObjectBaseClass(){

    x = 0; y = 0; zplane = db::Player;
    w = 0; h = 0; dx = 0; dy = 0;
    square_r = 0;

}

ObjectBaseClass::~ObjectBaseClass(){

}

void ObjectBaseClass::calculate_square_r()
{
    square_r = 0;
    for(unsigned int i = 0; i < bounding_points.size(); ++i){
        double r2 = (x-bounding_points[i][0])*(x-bounding_points[i][0]) + (y-bounding_points[i][1])*(y-bounding_points[i][1]);
        if(r2 > square_r) square_r = r2;
    }
}

void ObjectBaseClass::step_time(){
    x += dx;
    y += dy;
}


void ObjectBaseClass::drawon(SDL_Renderer *renderer, CameraControl *camera){
    (void)renderer;
    (void)camera;
}

void ObjectBaseClass::bounceoff_static(ObjectBaseClass* staticobject){
    (void)staticobject;
}
