/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/PegasusBody.hpp>

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
    : m_handle(UNKNOWN_ID)
    , m_scene(scene)
    , m_pPrimitive(nullptr)
{
    using pegasus::scene::Primitive;

    assert(nullptr != memento.pShape);

    pegasus::mechanics::Body body;

    if (memento.damping.first)
    {
        body.material.damping = memento.damping.second;
    }

    if (memento.linear.first)
    {
        body.linearMotion.position = memento.linear.second.position;
        body.linearMotion.velocity = memento.linear.second.velocity;
        body.linearMotion.acceleration = memento.linear.second.acceleration;
        body.linearMotion.force = memento.linear.second.force;
    }

    if (memento.angular.first)
    {
        body.angularMotion.orientation = memento.angular.second.orientation;
        body.angularMotion.velocity = memento.angular.second.velocity;
        body.angularMotion.acceleration = memento.angular.second.acceleration;
        body.angularMotion.torque = memento.angular.second.torque;
    }

    Primitive::Type primitiveType = Primitive::Type::DYNAMIC;

    if (memento.mass.first && !std::isnan(memento.mass.second))
    {
        body.material.SetMass(memento.mass.second);
    }
    else
    {
        body.material.SetInfiniteMass();
        primitiveType = Primitive::Type::STATIC;
    }

    {
        using Type = arion::SimpleShape::Type;

        switch (memento.pShape->type)
        {
            case Type::PLANE:
            {
                m_pPrimitive = new pegasus::scene::Plane(m_scene
                    , primitiveType
                    , body
                    , *static_cast<arion::Plane*>(memento.pShape)
                );

                break;
            }
            case Type::BOX:
            {
                arion::Box& shape = *static_cast<arion::Box*>(memento.pShape);

                m_pPrimitive = new pegasus::scene::Box(m_scene
                    , primitiveType
                    , body
                    , shape
                );

                body.material.SetMomentOfInertia(
                    pegasus::mechanics::CalculateSolidCuboidMomentOfInertia(
                        glm::length(shape.iAxis)
                        , glm::length(shape.jAxis)
                        , glm::length(shape.kAxis)
                        , body.material.GetMass()
                    )
                );

                break;
            }
            case Type::SPHERE:
            {
                arion::Sphere& shape = *static_cast<arion::Sphere*>(memento.pShape);

                m_pPrimitive = new pegasus::scene::Sphere(m_scene
                    , primitiveType
                    , body
                    , shape
                );

                body.material.SetMomentOfInertia(
                    pegasus::mechanics::CalculateSolidSphereMomentOfInertia(
                        shape.radius
                        , body.material.GetMass()
                    )
                );

                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (m_pPrimitive)
    {
        m_handle = m_pPrimitive->GetBodyHandle();
    }
}

BodyObject::~BodyObject()
{
    delete m_pPrimitive;
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

    m_primitives.push_back(pPrimitive);
    ++m_primitiveCount;

    m_collection.insert(pObj);

    return pObj;
}

BodyCollection::Object* BodyCollection::Get(Object::Memento const& memento) const
{
    assert(memento.handle != Object::UNKNOWN_ID);

    auto it = m_collection.find(memento.handle);

    return it != m_collection.end() ? *it : nullptr;
}

bool BodyCollection::Delete(Object::Memento const& memento)
{
    assert(memento.handle != Object::UNKNOWN_ID);

    auto it = m_collection.find(memento.handle);

    if (m_collection.end() != it)
    {
        m_collection.erase(it);

        return true;
    }
    else
    {
        return false;
    }
}

}
}
}
}
