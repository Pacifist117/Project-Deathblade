#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace deathblade{



/*!
 * \brief The ObjectBaseClass class is the highest level of object. It mainly contains
 * positioning information and virtual functions needed by all children.
 */
class ObjectBaseClass {

public:
    ObjectBaseClass();
    virtual ~ObjectBaseClass();

    virtual bool isMobile(){return false;}
    virtual bool isPlayer(){return false;}
    virtual void step_time();

    virtual void latch_to(ObjectBaseClass* other_object, vec2d relative_pos, double relative_th);
    virtual void release_latch();
    virtual void latched_by(ObjectBaseClass* other_object);
    virtual void latch_released(ObjectBaseClass* other_object);
    virtual bool is_latched();

    // Basic Private Member Manipulation
    virtual double& x(){return m_position.x();}
    virtual double& y(){return m_position.y();}
    virtual double& z(){return m_z;}
    virtual double& th(){return m_th;}

    virtual double& dx(){return m_velocity.x();}
    virtual double& dy(){return m_velocity.y();}
    virtual double& dz(){return m_dz;}
    virtual double& dth(){return m_dth;}

    virtual vec2d& vel(){return m_velocity;}
    virtual vec2d& pos(){return m_position;}

private:

    // Center of gravity info
    vec2d m_position; //!< World x,y position of object
    double m_z = 0; //!< World z point of object
    double m_th = 0; //!< World angle of object

    vec2d m_velocity; //!< World x,y speed of object (dx,dy)
    double m_dz = 0; //!< World z speed of object
    double m_dth = 0; //!< World angle speed of object

    // Object tracking
    ObjectBaseClass* m_latched_to = nullptr;
    vec2d m_latch_pos;
    double m_latch_th = 0;
    std::unordered_set<ObjectBaseClass*> m_latched_objects;
};

} // deathblade namespace

#endif // OBJECTBASE_H
