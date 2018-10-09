/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/system/Time.hpp>

#include <sleipnir/utility/Config.hpp>

#include <iostream>

namespace sleipnir
{
namespace system
{

Time::Time(WorldTime& worldTime
    , physics::Physics& physicsSystem
)
    : factor(1.0f)
    , m_realDuration(0)
    , m_worldDuration(0)
    , m_worldTime(worldTime)
    , m_physicsSystem(physicsSystem)
{

}

Time::TimeUnit Time::Update(TimeUnit realDuration)
{
    assert(factor > 0);

    m_realDuration = realDuration;

    TimeUnit const worldNow = m_worldTime.GetTime();
    TimeUnit const physNow = m_physicsSystem.GetCurrentTime();

    if ((worldNow - physNow) > utility::Config::PhysicsTick)
    {
        m_worldDuration = TimeUnit(0);
        std::cerr << "[Time] Physics are lagging behind "
            << std::chrono::microseconds(worldNow - physNow).count() << "us"
            << std::endl;
    }
    else
    {
        m_worldDuration = TimeUnit(static_cast<uint64_t>(static_cast<float>(realDuration.count()) * factor + 0.5f));
    }

    m_worldTime.SetTime(worldNow + m_worldDuration);

    return m_worldDuration;
}

}
}
