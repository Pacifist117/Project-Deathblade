#include "gameobjects/objectcontroller.h"
#include "SDL_rect.h"

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
    std::vector<ObjectBaseClass*>::iterator object1, object2;
    for (object1 = objects.begin(); object1 != objects.end(); ++object1){
        for (object2 = object1+1; object2 != objects.end(); ++object2){

            // If neither is moving, continue
            if (!(*object1)->isMobile() && !(*object2)->isMobile()) continue;

            // Calculate if possible collision
            double square_dist = ((*object1)->x - (*object2)->x)*((*object1)->x - (*object2)->x) +
                    ((*object1)->y - (*object2)->y)*((*object1)->y - (*object2)->y);
            if(square_dist > (*object1)->square_r + (*object2)->square_r) continue;

            if (!(*object1)->isMobile() || !(*object2)->isMobile()){
                ObjectBaseClass* mobileobject = ((*object1)->isMobile() ? (*object1) : (*object2));
                mobileobject->bounceoff_static((*object2));
            }
        }
    }
}
