#include "objectbase.h"

namespace deathblade{

void ObjectBaseClass::ObjectBaseClass()
{
}

void ObjectBaseClass::~ObjectBaseClass()
{
    release_latch();
    for(ObjectBaseClass* obj : m_latched_objects) obj->release_latch();
}

void ObjectBaseClass::step_time()
{
    if(is_latched())
    {
        pos() = m_latched_to->pos() + m_latch_pos;
        th() = m_latched_to->th() + m_latch_th;

        vel() = m_latched_to->vel();
        dth() = m_latched_to->th();
    }
    else
    {
        pos() += vel();
        th() += dth();
    }
}

void ObjectBaseClass::latch_to(ObjectBaseClass *other_object, vec2d relative_pos, double relative_th)
{
    release_latch();
    m_latched_to = other_object;
    m_latch_pos = relative_pos;
    m_latch_th = relative_th;

    if(other_object != nullptr) other_object->latched_by(this);
}

void ObjectBaseClass::release_latch()
{
    if(m_latched_to != nullptr)
    {
        m_latched_to->latch_released(this);
        m_latched_to = nullptr;
    }
}

void ObjectBaseClass::latched_by(ObjectBaseClass *other_object)
{
    m_latched_objects.insert(other_object);
}

void ObjectBaseClass::latch_released(ObjectBaseClass *other_object)
{
    m_latched_objects.erase(other_object);
}

bool ObjectBaseClass::is_latched()
{
    return m_latched_to != nullptr;
}



} // deathblade namespace
