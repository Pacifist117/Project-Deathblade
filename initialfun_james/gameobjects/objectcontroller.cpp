#include "objectcontroller.h"

ObjectController::ObjectController(){

}

ObjectController::~ObjectController(){

}

void ObjectController::step_time(){
    std::vector<ObjectBaseClass*>::iterator object;
    for (object = objects.begin(); object != objects.end(); ++object)
        (*object)->step_time();
}

void ObjectController::check_for_collisions(){

}
