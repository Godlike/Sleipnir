/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_LIFETIME_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_LIFETIME_COMPONENT_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/Types.hpp>

#include <chrono>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Lifetime description
struct LifetimeComponent : public Component
{
    //! Shortcut to time unit
    using Unit = WorldTime::TimeUnit;

    //! Basic constructor
    LifetimeComponent() : LifetimeComponent(Unit(0)) {}

    //! Constructor with given @ref deadline
    LifetimeComponent(Unit _deadline) : deadline(_deadline) {}

    //! Time when object shall be destroyed
    Unit deadline;
};

} // namespace component
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_COMPONENT_LIFETIME_COMPONENT_HPP
