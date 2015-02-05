#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "graphicscontrol/gameenums.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

#include <vector>

/*!
 * \brief The ObjectBaseClass class is the highest level of object. It mainly contains
 * positioning information and virtual functions needed by all children.
 */
class ObjectBaseClass {

public:
    ObjectBaseClass();
    virtual ~ObjectBaseClass();


    double x,y,w,h;
    double dx,dy;
    db::ZPlane zplane;
    std::vector<std::vector<double> > bounding_points;

    virtual void step_time();
    virtual void drawon(SDL_Renderer *renderer, SDL_Rect *destination);
    virtual bool isMobile(){return false;}

    virtual void bounceoff_static(ObjectBaseClass* staticobject);

};


#endif // OBJECTBASE_H