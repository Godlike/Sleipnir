/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/PegasusPrimitives.hpp>

#include <cassert>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

//! BodyObject::

BodyObject& BodyObject::operator+=(Memento const& memento)
{
    assert(m_handle == memento.handle);

    auto& body = m_scene.GetBody(m_handle);

    if (memento.linear.first)
    {
        assert(false == glm::isnan(memento.linear.second.position));
        assert(false == glm::isnan(memento.linear.second.velocity));
        assert(false == glm::isnan(memento.linear.second.acceleration));
        assert(false == glm::isnan(memento.linear.second.force));

        body.linearMotion.position += memento.linear.second.position;
        body.linearMotion.velocity += memento.linear.second.velocity;
        body.linearMotion.acceleration += memento.linear.second.acceleration;
        body.linearMotion.force += memento.linear.second.force;
    }

    if (memento.angular.first)
    {
        assert(false == glm::isnan(memento.angular.second.orientation));
        assert(false == glm::isnan(memento.angular.second.velocity));
        assert(false == glm::isnan(memento.angular.second.acceleration));
        assert(false == glm::isnan(memento.angular.second.torque));

        body.angularMotion.orientation += memento.angular.second.orientation;
        body.angularMotion.velocity += memento.angular.second.velocity;
        body.angularMotion.acceleration += memento.angular.second.acceleration;
        body.angularMotion.torque += memento.angular.second.torque;
    }

    if (memento.mass.first)
    {
        assert(false == std::isnan(memento.mass.second));

        body.material.SetMass(body.material.GetMass() + memento.mass.second);
    }

    if (memento.damping.first)
    {
        assert(false == std::isnan(memento.damping.second));

        body.material.damping += memento.damping.second;
    }

    return *this;
}

BodyObject& BodyObject::operator*=(Memento const& memento)
{
    assert(m_handle == memento.handle);

    auto& body = m_scene.GetBody(m_handle);

    if (memento.linear.first)
    {
        assert(false == glm::isnan(memento.linear.second.position));
        assert(false == glm::isnan(memento.linear.second.velocity));
        assert(false == glm::isnan(memento.linear.second.acceleration));
        assert(false == glm::isnan(memento.linear.second.force));

        body.linearMotion.position *= memento.linear.second.position;
        body.linearMotion.velocity *= memento.linear.second.velocity;
        body.linearMotion.acceleration *= memento.linear.second.acceleration;
        body.linearMotion.force *= memento.linear.second.force;
    }

    if (memento.angular.first)
    {
        assert(false == glm::isnan(memento.angular.second.orientation));
        assert(false == glm::isnan(memento.angular.second.velocity));
        assert(false == glm::isnan(memento.angular.second.acceleration));
        assert(false == glm::isnan(memento.angular.second.torque));

        body.angularMotion.orientation *= memento.angular.second.orientation;
        body.angularMotion.velocity *= memento.angular.second.velocity;
        body.angularMotion.acceleration *= memento.angular.second.acceleration;
        body.angularMotion.torque *= memento.angular.second.torque;
    }

    if (memento.mass.first)
    {
        assert(false == std::isnan(memento.mass.second));

        body.material.SetMass(body.material.GetMass() * memento.mass.second);
    }

    if (memento.damping.first)
    {
        assert(false == std::isnan(memento.damping.second));

        body.material.damping *= memento.damping.second;
    }

    return *this;
}

BodyObject::BodyObject(Memento const& memento, pegasus::scene::Scene& scene)
    : m_handle(memento.handle)
    , m_scene(scene)
{

}

//! BodyCollection::

BodyCollection::~BodyCollection()
{
    for (auto& v : m_collection)
    {
        delete m_collection;
    }
}

BodyCollection::Object* BodyCollection::Spawn(Object::Memento const& memento)
{
    assert(memento.handle == Object::UNKNOWN_ID);

    Object* pObj = new Object(memento, m_scene);
    pObj->m_handle = pPrimitive->GetHandle();

    m_collection.push_back(pObj);

    return pObj;
}



}
}
}
}
