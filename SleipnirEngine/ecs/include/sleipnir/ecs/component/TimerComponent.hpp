/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_TIMER_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_TIMER_COMPONENT_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/entity/Entity.hpp>

#include <sleipnir/ecs/Types.hpp>

#include <wink/signal.hpp>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Timer description
struct TimerComponent : public Component
{
    //! Last processed time
    WorldTime::TimeUnit lastTime;

    //! Time point when timer times out
    WorldTime::TimeUnit endTime;

    //! Timer tick duration
    WorldTime::TimeUnit tick;

    //! Callback to be triggered on each tick
    wink::signal< wink::slot<void(entity::Entity)> > onTick;

    //! Callback to be called upon reaching @ref endTime
    wink::signal< wink::slot<void(entity::Entity)> > onTimeout;
};

} // namespace component
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_COMPONENT_TIMER_COMPONENT_HPP
