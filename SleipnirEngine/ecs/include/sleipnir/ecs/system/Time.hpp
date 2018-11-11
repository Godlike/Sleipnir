/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_TIME_HPP
#define SLEIPNIR_ECS_SYSTEM_TIME_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/system/physics/Physics.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{

/** @brief  Custom system responsible for time flow control
 *
 *  In addition to time factor control, this system would throttle time
 *  whenever physics system is unable to catch up to world time
 */
class Time
{
public:
    //! Shortcut to time unit
    using TimeUnit = WorldTime::TimeUnit;

    /** @brief  Basic constructor
     *
     *  @param  worldTime       time holder
     *  @param  physicsSystem   physics system
     */
    Time(WorldTime& worldTime, physics::Physics& physicsSystem);

    //! Default destsructor
    ~Time() = default;

    /** @brief  Method invoked each loop cycle
     *
     *  Ensures that physics was able to catch up to current time, throttles
     *  time flow if physics is lagging behind.
     *  Multiplies @p realDuration with @ref factor to calculate world time
     *  duration of current frame.
     *
     *  Updates @ref m_worldTime with calculated world time duration.
     *
     *  @param  realDuration    raw duration of current frame
     *
     *  @return world time duration
     */
    TimeUnit Update(TimeUnit realDuration);

    //! Returns raw duration of current frame
    TimeUnit GetRealDuration() const { return m_realDuration; }

    //! Returns world duration of current frame
    TimeUnit GetWorldDuration() const { return m_worldDuration; }

    //! Time flow factor
    float factor;

private:
    //! Raw duration of current frame
    TimeUnit m_realDuration;

    //! World duration of current frame
    TimeUnit m_worldDuration;

    //! Time holder
    WorldTime& m_worldTime;

    //! Physics system
    physics::Physics& m_physicsSystem;
};

} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_TIME_HPP
