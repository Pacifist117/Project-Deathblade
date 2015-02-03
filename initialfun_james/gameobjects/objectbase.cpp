#include "objectbase.h"

ObjectBaseClass::ObjectBaseClass(TempSettings *gamesettings){

    if (gamesettings == NULL){
        std::cerr << "A null pointer was passed into object base class\n";
        return;
    }

    game_settings = gamesettings;

    x = 0; y = 0; zplane = CameraControl::ZPlane::Player;
    w = 0; h = 0; dx = 0; dy = 0;

}

ObjectBaseClass::~ObjectBaseClass(){

}

void ObjectBaseClass::step_time(){
    x += dx;
    y += dy;
}
