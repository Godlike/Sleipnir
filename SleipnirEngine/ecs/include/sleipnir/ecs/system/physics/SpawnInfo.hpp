/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_SPAWN_INFO_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_SPAWN_INFO_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/Types.hpp>

#include <pegasus/Scene.hpp>

#include <Arion/Shape.hpp>

namespace sleipnir
{
namespace ecs
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
};

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_SPAWN_INFO_HPP
