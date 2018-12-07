/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/TimeWithPhysics.hpp>

#include <sleipnir/utility/Config.hpp>
#include <sleipnir/utility/InternalLoggers.hpp>

#include <cassert>
#include <iostream>

namespace sleipnir
{
namespace ecs
{
namespace system
{

TimeWithPhysics::TimeWithPhysics(WorldTime& worldTime
    , physics::Physics& physicsSystem
)
    : TimeBase(worldTime)
    , m_physicsSystem(physicsSystem)
{

}

TimeWithPhysics::TimeUnit TimeWithPhysics::Update(TimeUnit realDuration)
{
    assert(m_factor > 0);

    m_realDuration = realDuration;

    TimeUnit const worldNow = m_worldTime.GetTime();
    TimeUnit const physNow = m_physicsSystem.GetCurrentTime();

    if ((worldNow - physNow) > utility::Config::PhysicsTick)
    {
        m_worldDuration = TimeUnit(0);

        LOG->Error("[TimeWithPhysics] Physics are lagging behind {}us", std::chrono::microseconds(worldNow - physNow).count());
    }
    else
    {
        m_worldDuration = TimeUnit(static_cast<uint64_t>(static_cast<float>(realDuration.count()) * m_factor + 0.5f));
    }

    m_worldTime.SetTime(worldNow + m_worldDuration);

    return m_worldDuration;
}

} // namespace system
} // namespace ecs
} // namespace sleipnir
