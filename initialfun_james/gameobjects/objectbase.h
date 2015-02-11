#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "graphicscontrol/gameenums.h"
#include "graphicscontrol/cameracontrol.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

#include <vector>

class vec2d {

public:
    vec2d(){x=0;y=0;}
    vec2d(double x, double y){
        this->x = x;
        this->y = y;
    }
    double x,y;

    double dot(const vec2d other){ return x*other.x + y*other.y; }
    vec2d operator+(const vec2d other){ return vec2d(x+other.x,y+other.y); }
    vec2d operator-(const vec2d other){ return vec2d(x-other.x,y-other.y); }
    vec2d operator*(const double mag){ return vec2d(x*mag, y*mag); }
    double &operator[](unsigned int n){ return (n==0 ? x : y); }

};


/*!
 * \brief The ObjectBaseClass class is the highest level of object. It mainly contains
 * positioning information and virtual functions needed by all children.
 */
class ObjectBaseClass {

public:
    ObjectBaseClass();
    virtual ~ObjectBaseClass();


    double x,y;
    double dx,dy,dth;
    double mass, elasticity, friction;
    db::ZPlane zplane;
    std::vector<vec2d> side_normals;
    std::vector<vec2d> bounding_points;
    void translate(double delx, double dely);
    void rotate(double deltheta);

    /*!
     * \brief organize_points
     * \bug square_r method does not work well. Should replace with quad tree?
     */
    void organize_points();
    void calculate_normals();
    vec2d project_onto(vec2d &axis);
    double square_r;

    virtual void step_time();
    virtual void drawon(SDL_Renderer* renderer, CameraControl* camera);
    virtual bool isMobile(){return false;}

private:

    struct clockwise {
        clockwise(const ObjectBaseClass& c) : theshape(c) {}
        bool operator () (const vec2d& a, const vec2d& b){
            double atheta = atan2(a.y-theshape.y,a.x-theshape.x);
            double btheta = atan2(b.y-theshape.y,b.x-theshape.x);
            return atheta < btheta;
        }
        const ObjectBaseClass& theshape;
    };
};


#endif // OBJECTBASE_H
