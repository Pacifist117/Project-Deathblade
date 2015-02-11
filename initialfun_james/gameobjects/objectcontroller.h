#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H

#include "graphicscontrol/controlbaseclass.h"
#include "gameobjects/objectbase.h"
#include <vector>

/*!
 * \brief The ObjectController class routinely updates all objects and checks for intersections.
 */
class ObjectController {

public:
    ObjectController();
    ~ObjectController();

    std::vector<ObjectBaseClass*> objects;

    void step_time();
    void check_for_collisions();
    bool colliding(ObjectBaseClass* a, ObjectBaseClass* b, vec2d &return_normal, double &overlap);
    void one_mobile_bounce(ObjectBaseClass* mobile, ObjectBaseClass* nonmobile, vec2d mtv_unit, double mtv_mag);
    void two_mobiles_bounce(ObjectBaseClass* a, ObjectBaseClass* b, vec2d mtv_unit, double mtv_mag);
    void add_object(ObjectBaseClass* newobject);

};


#endif // OBJECTCONTROLLER_H
