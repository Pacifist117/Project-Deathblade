#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "../graphicscontrol/cameracontrol.h" // temporarily for tempsettings

/*!
 * \brief The ObjectBaseClass class is the highest level of object. It mainly contains
 * positioning information and virtual functions needed by all children.
 */
class ObjectBaseClass {

public:
    ObjectBaseClass(TempSettings *gamesettings);
    ~ObjectBaseClass();

    TempSettings *game_settings;

    double x,y,w,h;
    double dx,dy;
    CameraControl::ZPlane zplane;

    virtual void step_time();

};


#endif // OBJECTBASE_H
