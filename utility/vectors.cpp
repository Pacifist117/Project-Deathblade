#include "vectors.h"

#include <iostream>

namespace deathblade{

vec2d::vec2d(){}
vec2d::vec2d(double x, double y) : m_x(x),m_y(y){}

double& vec2d::x(){return m_x;}
double& vec2d::y(){return m_y;}
double  vec2d::r(){return sqrt(m_x*m_x + m_y*m_y);}
double vec2d::r2(){return m_x*m_x + m_y*m_y;}


bool vec2d::is_in(std::vector<vec2d>& points)
{
    return ( std::find(points.begin(),points.end(), *this) == points.end() );
}

void vec2d::normalize()
{
    double radius = r();
    m_x /= radius;
    m_y /= radius;
}

void vec2d::rotate(double theta)
{
    double tempx = m_x;
    m_x = m_x*cos(theta) - m_y*sin(theta);
    m_y = tempx*sin(theta) + m_y*cos(theta);
}

void vec2d::rotate_about(double theta, const vec2d& origin)
{
    vec2d relative_point  = *this - origin;
    relative_point.rotate(theta);
    *this = origin + relative_point;
}

friend double vec2d::dot(const vec2d& a, const vec2d& b){ return a.m_x*b.m_x + a.m_y*b.m_y; }
friend bool vec2d::operator==(const vec2d& a, const vec2d& b){ return a.m_x==b.m_x && a.m_y==b.m_y; }
friend vec2d vec2d::operator-(const vec2d& a, const vec2d& b){ return vec2d(a.m_x-b.m_x,a.m_y-b.m_y); }
friend vec2d vec2d::operator+(const vec2d& a, const vec2d& b){ return vec2d(a.m_x+b.m_x,a.m_y+b.m_y); }
friend vec2d vec2d::operator*(const vec2d& a, const vec2d& b){ return vec2d(a.m_x*b.m_x,a.m_y*b.m_y); }
friend vec2d vec2d::operator/(const vec2d& a, const vec2d& b){ return vec2d(a.m_x/b.m_x,a.m_y/b.m_y); }

friend vec2d vec2d::operator*(const vec2d& a, double mag){return vec2d(a.m_x*mag,a.m_y*mag);}
friend vec2d vec2d::operator*(double mag, const vec2d& a){return vec2d(a.m_x*mag,a.m_y*mag);}
friend vec2d vec2d::operator/(const vec2d& a, double mag){return vec2d(a.m_x/mag,a.m_y/mag);}
friend vec2d vec2d::operator/(double mag, const vec2d& a){return vec2d(mag/a.m_x,mag/a.m_y);}
friend vec2d vec2d::operator+(const vec2d& a, double mag){return vec2d(a.m_x+mag,a.m_y+mag);}
friend vec2d vec2d::operator+(double mag, const vec2d& a){return vec2d(a.m_x+mag,a.m_y+mag);}
friend vec2d vec2d::operator-(const vec2d& a, double mag){return vec2d(a.m_x-mag,a.m_y-mag);}
friend vec2d vec2d::operator-(double mag, const vec2d& a){return vec2d(mag-a.m_x,mag-a.m_y);}

void vec2d::operator=(double mag){ m_x=mag; m_y=mag; }



friend std::ostream& vec2d::operator<<(std::ostream& os, const vec2d& a){
    os << a.m_x << " " << a.m_y;
    return os;
}
friend std::istream& vec2d::operator>>(std::istream& is, vec2d& a){
    is >> a.m_x >> a.m_y;
    return is;
}

vec2d vec2d::operator-(){ return vec2d(-m_x, -m_y); }
double &vec2d::operator[](const unsigned int& n){ return (n==0 ? m_x : m_y); }


} // deathblade namespace
