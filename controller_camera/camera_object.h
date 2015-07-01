#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H


namespace deathblade{

#include "gameobjects/objectbase.h"

class CameraObject : public ObjectBaseClass {

    ObjectBaseClass* tracking_object = nullptr;
};

class CameraCalculator {

};

} // deathblade namespace
#endif
