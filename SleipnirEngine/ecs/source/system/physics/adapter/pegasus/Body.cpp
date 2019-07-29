/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/adapter/pegasus/Body.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{
namespace adapter
{
namespace pegasus
{

namespace
{
    void ApplyMemento(::pegasus::mechanics::Body& body, BodyMemento const& memento)
    {
        if (memento.linear.first)
        {
            BodyMemento::LinearMotion const& linearMotion = memento.linear.second;

            body.linearMotion.position = linearMotion.position;
            body.linearMotion.velocity = linearMotion.velocity;
            body.linearMotion.acceleration = linearMotion.acceleration;
            body.linearMotion.force = linearMotion.force;
        }

        if (memento.angular.first)
        {
            BodyMemento::AngularMotion const& angularMotion = memento.angular.second;

            body.angularMotion.orientation = angularMotion.orientation;
            body.angularMotion.velocity = angularMotion.velocity;
            body.angularMotion.acceleration = angularMotion.acceleration;
            body.angularMotion.torque = angularMotion.torque;
        }

        if (memento.mass.first)
        {
            if (!std::isnan(memento.mass.second))
            {
                body.material.SetMass(memento.mass.second);
            }
            else
            {
                body.material.SetInfiniteMass();
            }
        }

        if (memento.damping.first)
        {
            assert(!std::isnan(memento.damping.second));
            body.material.damping = memento.damping.second;
        }
    }

    void AddMemento(::pegasus::mechanics::Body& body, BodyMemento const& memento)
    {
        if (memento.linear.first)
        {
            BodyMemento::LinearMotion const& linearMotion = memento.linear.second;

            body.linearMotion.position += linearMotion.position;
            body.linearMotion.velocity += linearMotion.velocity;
            body.linearMotion.acceleration += linearMotion.acceleration;
            body.linearMotion.force += linearMotion.force;
        }

        if (memento.angular.first)
        {
            BodyMemento::AngularMotion const& angularMotion = memento.angular.second;

            body.angularMotion.orientation += angularMotion.orientation;
            body.angularMotion.velocity += angularMotion.velocity;
            body.angularMotion.acceleration += angularMotion.acceleration;
            body.angularMotion.torque += angularMotion.torque;
        }

        if (memento.mass.first)
        {
            if (!(std::isnan(memento.mass.second) || body.material.HasInfiniteMass()))
            {
                body.material.SetMass(body.material.GetMass() + memento.mass.second);
            }
            else
            {
                body.material.SetInfiniteMass();
            }
        }

        if (memento.damping.first)
        {
            assert(!std::isnan(memento.damping.second));
            body.material.damping += memento.damping.second;
        }
    }

    void MultiplyMemento(::pegasus::mechanics::Body& body, BodyMemento const& memento)
    {
        if (memento.linear.first)
        {
            BodyMemento::LinearMotion const& linearMotion = memento.linear.second;

            body.linearMotion.position *= linearMotion.position;
            body.linearMotion.velocity *= linearMotion.velocity;
            body.linearMotion.acceleration *= linearMotion.acceleration;
            body.linearMotion.force *= linearMotion.force;
        }

        if (memento.angular.first)
        {
            BodyMemento::AngularMotion const& angularMotion = memento.angular.second;

            body.angularMotion.orientation *= angularMotion.orientation;
            body.angularMotion.velocity *= angularMotion.velocity;
            body.angularMotion.acceleration *= angularMotion.acceleration;
            body.angularMotion.torque *= angularMotion.torque;
        }

        if (memento.mass.first)
        {
            if (!(std::isnan(memento.mass.second) || body.material.HasInfiniteMass()))
            {
                body.material.SetMass(body.material.GetMass() * memento.mass.second);
            }
            else
            {
                body.material.SetInfiniteMass();
            }
        }

        if (memento.damping.first)
        {
            assert(!std::isnan(memento.damping.second));
            body.material.damping *= memento.damping.second;
        }
    }
}

// BodyObject::

BodyObject& BodyObject::operator=(Memento const& memento)
{
    ::pegasus::mechanics::Body& body = m_scene.GetBody(m_handle);

    ApplyMemento(body, memento);

    return *this;
}

BodyObject& BodyObject::operator+=(Memento const& memento)
{
    ::pegasus::mechanics::Body& body = m_scene.GetBody(m_handle);

    AddMemento(body, memento);

    return *this;
}

BodyObject& BodyObject::operator*=(Memento const& memento)
{
    ::pegasus::mechanics::Body& body = m_scene.GetBody(m_handle);

    MultiplyMemento(body, memento);

    return *this;
}

glm::vec3 BodyObject::GetPosition() const
{
    return m_scene.GetBody(m_handle).linearMotion.position;
}

BodyObject::BodyObject(Memento const& memento
    , BodyCollection* pParent
    , ::pegasus::scene::Scene& scene
)
    : m_handle(UNKNOWN_ID)
    , m_scene(scene)
    , m_pPrimitive(nullptr)
{
    if (memento.pShape)
    {
        {
            using ::pegasus::scene::Primitive;
            using Type = arion::SimpleShape::Type;

            ::pegasus::mechanics::Body body;
            ApplyMemento(body, memento);

            Primitive::Type const primitiveType = (!body.material.HasInfiniteMass() ? Primitive::Type::DYNAMIC : Primitive::Type::STATIC);

            switch (memento.pShape->type)
            {
                case Type::PLANE:
                {
                    m_pPrimitive = new ::pegasus::scene::Plane(m_scene
                        , primitiveType
                        , body
                        , *static_cast<arion::Plane*>(memento.pShape)
                    );

                    break;
                }
                case Type::BOX:
                {
                    arion::Box& shape = *static_cast<arion::Box*>(memento.pShape);

                    body.material.SetMomentOfInertia(
                        ::pegasus::mechanics::CalculateSolidCuboidMomentOfInertia(
                            glm::length(shape.iAxis)
                            , glm::length(shape.jAxis)
                            , glm::length(shape.kAxis)
                            , body.material.GetMass()
                        )
                    );

                    m_pPrimitive = new ::pegasus::scene::Box(m_scene
                        , primitiveType
                        , body
                        , shape
                    );

                    break;
                }
                case Type::SPHERE:
                {
                    arion::Sphere& shape = *static_cast<arion::Sphere*>(memento.pShape);

                    body.material.SetMomentOfInertia(
                        ::pegasus::mechanics::CalculateSolidSphereMomentOfInertia(
                            shape.radius
                            , body.material.GetMass()
                        )
                    );

                    m_pPrimitive = new ::pegasus::scene::Sphere(m_scene
                        , primitiveType
                        , body
                        , shape
                    );

                    break;
                }
                default:
                {
                    assert(false);
                    break;
                }
            }
        }
    }

    assert(nullptr != m_pPrimitive);

    m_handle = m_pPrimitive->GetBodyHandle();
}

BodyObject::~BodyObject()
{
    delete m_pPrimitive;
}

// BodyCollection::

BodyCollection::BodyCollection(::pegasus::scene::Scene& scene)
    : m_scene(scene)
    , m_primitiveCount(0)
{

}

BodyCollection::~BodyCollection()
{
    for (auto [key, value] : m_collection)
    {
        delete value;
    }
}

BodyCollection::Object* BodyCollection::Spawn(BodyHandle* pHandle, Object::Memento const& memento)
{
    assert(nullptr != pHandle);
    assert(BodyMemento::UNKNOWN_ID == memento.handle);

    Object* pObj = new Object(memento, this, m_scene);

    assert(BodyMemento::UNKNOWN_ID != pObj->m_handle);
    assert(m_collection.cend() == m_collection.find(pObj->m_handle));

    pHandle->bodyHandle.store(pObj->m_handle);
    m_collection.emplace(pObj->m_handle, pObj);

    m_primitives.push_back(pObj->m_pPrimitive);
    ++m_primitiveCount;

    return pObj;
}

BodyCollection::Object* BodyCollection::Get(BodyHandle* pHandle) const
{
    auto cit = m_collection.find(pHandle->bodyHandle.load());

    return ((m_collection.cend() != cit) ? cit->second : nullptr);
}

bool BodyCollection::Delete(BodyHandle* pHandle)
{
    assert(nullptr != pHandle);

    auto it = m_collection.find(pHandle->bodyHandle.load());

    if (m_collection.end() != it)
    {
        assert(nullptr != it->second);

        Object::Handle bodyHandle = it->second->m_handle;

        auto primitiveIt = std::find_if(
            m_primitives.begin()
            , m_primitives.end()
            , [=](auto const& ptr) -> bool
            {
                return ptr->GetBodyHandle() == bodyHandle;
            }
        );

        if (m_primitives.end() != primitiveIt)
        {
            m_primitives.erase(primitiveIt);

            --m_primitiveCount;
        }

        delete it->second;
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
}
}