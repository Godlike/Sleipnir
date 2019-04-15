/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP

#include <utility>

#include <pegasus/Asset.hpp>
#include <pegasus/Scene.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

class BodyObject;
class BodyCollection;

struct BodyMemento
{
    //! Shortcut to associated object type
    using Object = BodyObject;

    //! Shortcut to underlying handle type
    using Handle = pegasus::scene::Handle;

    //! ID to be used when body handle is unknown/unavailable
    constexpr static Handle UNKNOWN_ID = pegasus::scene::ZERO_HANDLE;

    struct LinearMotion
    {
        glm::vec3 position = {0};
        glm::vec3 velocity = {0};
        glm::vec3 acceleration = {0};
        glm::vec3 force = {0};
    };

    struct AngularMotion
    {
        glm::quat orientation = {0};
        glm::vec3 velocity = {0};
        glm::vec3 acceleration = {0};
        glm::vec3 torque = {0};
    };

    //! Pegasus body handle
    pegasus::scene::Handle handle = Object::UNKNOWN_ID;

    //! Pointer to arion shape
    arion::SimpleShape* pShape = nullptr;

    //! Body linear motion
    std::pair<bool, LinearMotion> linear = {false, {}};

    //! Body angular motion
    std::pair<bool, AngularMotion> angular = {false, {}};

    //! Mass
    std::pair<bool, double> mass = {false, 0.0};

    //! Damping
    std::pair<bool, double> damping = {false, 0.0};
};

class BodyObject
{
public:
    using Memento = BodyMemento;
    using Handle = pegasus::scene::Handle;
    constexpr static Handle UNKNOWN_ID = Memento::UNKNOWN_ID;

    BodyObject(BodyObject const& other) = default;
    BodyObject& operator=(BodyObject const& other) = default;

    BodyObject(BodyObject&& other) = default;
    BodyObject& operator=(BodyObject&& other) = default;

    ~BodyObject() = default;

    BodyObject& operator+=(Memento const& memento);
    BodyObject& operator*=(Memento const& memento);

private:
    friend class BodyCollection;

    BodyObject(Memento const& memento, pegasus::scene::Scene& scene);

    Handle m_handle;
    pegasus::scene::Scene& m_scene;
};

class BodyCollection
{
public:
    using Object = BodyObject;

    BodyCollection() = default;

    BodyCollection(BodyCollection const& other) = delete;
    BodyCollection& operator=(BodyCollection const& other) = delete;

    BodyCollection(BodyCollection&& other) = delete;
    BodyCollection& operator=(BodyCollection&& other) = delete;

    ~BodyCollection();

    Object* Spawn(Object::Memento const& memento);
    Object* Get(Object::Memento const& memento) const;
    bool Delete(Object::Memento const& memento);

private:
    //! Shortcut to a collection of pegasus::scene::Primitive pointers
    using Primitives = std::list<pegasus::scene::Primitive*>;

    //! Pegasus world
    pegasus::scene::Scene m_scene;

    //! Container for created objects
    std::vector<Object*> m_collection;

    //! Collection of primitives
    Primitives m_primitives;

    //! Amount of primitives in @p primitives
    std::size_t m_primitiveCount;
};

using BodyChanges = sleipnir::utility::cc::Changes<BodyObject::Memento>;
using BodyIntegrator = BodyChanges::Integrator<BodyCollection>;

}
}
}
}

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP
