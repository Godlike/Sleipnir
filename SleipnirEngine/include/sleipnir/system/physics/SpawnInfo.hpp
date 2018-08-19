/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_SPAWN_INFO_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_SPAWN_INFO_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/util/Types.hpp>

#include <Arion/Shape.hpp>

namespace sleipnir
{
namespace system
{
namespace physics
{

//! Body information for spawner
struct SpawnInfo
{
    //! Pointer to arion shape
    arion::SimpleShape* pShape;

    //! Initial body velocity
    glm::dvec3 velocity;

    //! Mass
    double mass;

    //! Damping
    double damping;

    //! Static force that body should be bound to
    Force force;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_SPAWN_INFO_HPP
