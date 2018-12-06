/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/TimeBase.hpp>

#include <cassert>

namespace sleipnir
{
namespace ecs
{
namespace system
{

TimeBase::TimeBase(WorldTime& worldTime)
    : m_factor(1.0f)
    , m_realDuration(0)
    , m_worldDuration(0)
    , m_worldTime(worldTime)
{

}

TimeBase::TimeUnit TimeBase::Update(TimeUnit realDuration)
{
    assert(m_factor > 0);

    m_realDuration = realDuration;

    TimeUnit const worldNow = m_worldTime.GetTime();

    m_worldDuration = TimeUnit(static_cast<uint64_t>(static_cast<float>(realDuration.count()) * m_factor + 0.5f));

    m_worldTime.SetTime(worldNow + m_worldDuration);

    return m_worldDuration;
}

} // namespace system
} // namespace ecs
} // namespace sleipnir
