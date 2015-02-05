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

};


#endif // OBJECTCONTROLLER_H
