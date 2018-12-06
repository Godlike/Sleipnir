/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_TIME_WITH_PHYSICS_HPP
#define SLEIPNIR_ECS_SYSTEM_TIME_WITH_PHYSICS_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/system/TimeBase.hpp>
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
class TimeWithPhysics : public TimeBase
{
public:
    //! Shortcut to time unit
    using TimeUnit = TimeBase::TimeUnit;

    /** @brief  Basic constructor
     *
     *  @param  worldTime       time holder
     *  @param  physicsSystem   physics system
     */
    TimeWithPhysics(WorldTime& worldTime, physics::Physics& physicsSystem);

    //! Default destsructor
    ~TimeWithPhysics() = default;

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
    virtual TimeUnit Update(TimeUnit realDuration) override;

private:
    //! Physics system
    physics::Physics& m_physicsSystem;

};

} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_TIME_WITH_PHYSICS_HPP
