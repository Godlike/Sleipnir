/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_TIMER_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_TIMER_COMPONENT_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/entity/Entity.hpp>

#include <sleipnir/utility/Types.hpp>

#include <wink/signal.hpp>

namespace sleipnir
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

}
}

#endif // SLEIPNIR_COMPONENT_TIMER_COMPONENT_HPP
