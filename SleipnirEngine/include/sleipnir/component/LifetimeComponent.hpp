/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_LIFETIME_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_LIFETIME_COMPONENT_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/utility/Types.hpp>

#include <chrono>

namespace sleipnir
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

}
}

#endif // SLEIPNIR_COMPONENT_LIFETIME_COMPONENT_HPP
