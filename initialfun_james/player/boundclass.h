#ifndef BOUNDCLASS_H
#define BOUNDCLASS_H

#include "gameobjects/objectbase.h"
#include "graphicscontrol/cameracontrol.h"

class BoundClass
{
public:
    BoundClass();
    ~BoundClass();

    double xclick;
    double yclick;
    double xdrag;
    double ydrag;

    //ObjectBaseClass* bound_object;

    void drawon(SDL_Renderer* renderer, CameraControl* camera);
    bool enabled;


};

#endif // BOUNDCLASS_H
