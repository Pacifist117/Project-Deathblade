#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "gameobjects/objectbase.h"

#include "controller_base/gameenums.h"
#include "controller_camera/cameracontrol.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

#include <vector>


namespace deathblade{



/*!
 * \brief The GameObjectClass class is the highest level of object. It mainly contains
 * positioning information and virtual functions needed by all children.
 */
class GameObjectClass : public ObjectBaseClass {

public:
    GameObjectClass();
    virtual ~GameObjectClass();

    /*!
     * \brief Moves the object, namely shifts values x,y and bounding points.
     * \param delx Change in x to move.
     * \param dely Change in y to move.
     */
    void translate(const double& delx, const double& dely);

    /*!
     * \brief Rotates the object, namely shifts th.
     * \param deltheta Change in theta to rotate (positive is clockwise).
     */
    void rotate(const double& deltheta);

    /*!
     * \brief Sorts the points and calls calculate_normals().
     * \bug square_r method does not work well. Should replace with quad tree?
     */
    void organize_points();

    /*!
     * \brief Uses bounding_points to calculate normal vectors to the edges of the object.
     */
    void calculate_normals();

    /*!
     * \brief Projects the object onto an axis, used in collision.
     * \param axis Axis to be projected onto.
     * \return Two dimensional vector with [0]/x being the minimum on axis and [1]/y the maximum.
     */
    vec2d project_onto(const vec2d &axis);


    // Inherited members.
    virtual void drawon(SDL_Renderer* renderer, CameraControl* camera) = 0;

    // Private member manipulation
    virtual double& mass(){ return m_mass; }
    virtual double& elasticity(){ return m_elasticity; }
    virtual double& friction(){ return m_friction; }
    virtual double& square_r(){ return m_square_r; }

private:


    double m_mass = 1;          //!< Characteristics of object related to collision.
    double m_elasticity = 1;
    double m_friction = 0;
    std::vector<vec2d> m_side_normals; //!< Normal vectors of the bounding line segments, used in collision.
    std::vector<vec2d> m_bounding_points; //!< Points bounding the object, used in collision.
    double m_square_r = 0; //!< Maximum radius squared. \bug m_square_r is not working

};


} // deathblade namespace
#endif // OBJECTBASE_H
