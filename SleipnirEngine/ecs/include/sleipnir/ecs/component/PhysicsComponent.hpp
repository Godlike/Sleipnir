/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_PHYSICS_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_PHYSICS_COMPONENT_HPP

#include <sleipnir/ecs/system/physics/PhysicsThread.hpp>

#include <sleipnir/ecs/Types.hpp>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Physics description
struct PhysicsComponent : public Component
{
    //! Physics body handle
    system::physics::BodyHandle* pHandle;
};

} // namespace component
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_COMPONENT_PHYSICS_COMPONENT_HPP
