/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_TIME_BASE_HPP
#define SLEIPNIR_ECS_SYSTEM_TIME_BASE_HPP

#include <sleipnir/ecs/WorldTime.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{

//! Time system base class
class TimeBase
{
public:
    //! Shortcut to time unit
    using TimeUnit = WorldTime::TimeUnit;

    //! Default constructor
    TimeBase(WorldTime& worldTime);

    //! Default destructor
    virtual ~TimeBase() = default;

    /** @brief  Method invoked each loop cycle
     *
     *  Multiplies @p realDuration with @ref factor to calculate world time
     *  duration of current frame.
     *
     *  Updates @ref m_worldTime with calculated world time duration.
     *
     *  @param  realDuration    raw duration of current frame
     *
     *  @return world time duration
     */
    virtual TimeUnit Update(TimeUnit realDuration);

    //! Returns raw duration of current frame
    TimeUnit GetRealDuration() const { return m_realDuration; }

    //! Returns world duration of current frame
    TimeUnit GetWorldDuration() const { return m_worldDuration; }

    //! Set time flow factor
    void SetFactor(float factor) { m_factor = factor; }

    //! Returns time flow factor
    float GetFactor() const { return m_factor; }

protected:
    //! Time flow factor
    float m_factor;

    //! Raw duration of current frame
    TimeUnit m_realDuration;

    //! World duration of current frame
    TimeUnit m_worldDuration;

    //! Time holder
    WorldTime& m_worldTime;

};

} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_TIME_BASE_HPP
