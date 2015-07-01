#include "gameobject.h"

#include <algorithm>

namespace deathblade{


GameObjectClass::GameObjectClass(){}

GameObjectClass::~GameObjectClass(){}




// dealing with points

void GameObjectClass::organize_points(){

    std::sort(bounding_points.begin(),bounding_points.end(),
      [](const vec2d& a, const vec2d& b) { return
        atan2(a.y()-y(),a.x()-x()) < atan2(b.y()-y(),b.x()-x());
    });

    calculate_normals();

    square_r() = 0;
    for (auto&& bp : m_bounding_points){
        double r2 = (bp - vec2d(x(),y())).r2();
        if(r2 > square_r()) square_r() = r2;
    }
}







void GameObjectClass::calculate_normals(){

    for(unsigned int i = 0; i < bounding_points.size(); ++i)
    {
        vec2d& next_point = m_bounding_points[ (i+1==m_bounding_points.size()) ? 0 : i+1 ];
        vec2d normal = m_bounding_points[i] - next_point;

        normal.normalize();

        if( !normal.is_in(m_side_normals) && !(-normal).is_in(m_side_normals) )
            m_side_normals.push_back(normal);
    }
}







vec2d GameObjectClass::project_onto(const vec2d& axis)
{
    vec2d minmax(-10e10,-10e10);

    if(m_bounding_points.size() == 0){
        return minmax;
    }


    minmax[0] = axis.dot(m_bounding_points[0]);
    minmax[1] = minmax[0];


    for(auto&& bp : m_bounding_points)
    {
        double p = axis.dot(bp);
        if(p < minmax[0]) minmax[0] = p;
        else if(p > minmax[1]) minmax[1] = p;
    }

    return minmax;
}






void GameObjectClass::translate(const double &delx, const double &dely){

    if(delx == 0 && dely == 0) return;

    x() += delx;
    y() += dely;

    for (auto&& bp : m_bounding_points) {bp += vec2d(delx,dely);}

}





void GameObjectClass::rotate(const double& deltheta){

    if(deltheta == 0) return;

    th += deltheta;
    for (auto& bp : m_bounding_points) bp.rotate_about(deltheta, vec2d(x(),y()));
    for (auto& normal : m_side_normals) normal.rotate(deltheta);

}


} // deathblade namespace
