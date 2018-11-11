/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/Systems.hpp>

#include <sleipnir/ecs/system/Time.hpp>
#include <sleipnir/ecs/system/physics/Physics.hpp>

#include <assert.h>

namespace sleipnir
{
namespace ecs
{

struct Systems::BuiltInSystems
{
    BuiltInSystems(entity::World& world, WorldTime& worldTime)
        : physics(world, worldTime)
        , time(worldTime, physics)
    {}

    system::physics::Physics physics;
    system::Time time;
};

Systems::Systems(entity::World& world, WorldTime& worldTime)
    : m_pBuiltInSystems(new BuiltInSystems(world, worldTime))
{
    Add(&(m_pBuiltInSystems->physics), static_cast<uint16_t>(DefaultPriority::Physics));
}

Systems::~Systems()
{
    delete m_pBuiltInSystems;
}

void Systems::Add(system::ISystem* pSystem, uint16_t priority)
{
    assert(nullptr != pSystem);

    m_systems.emplace(Entry{ priority, pSystem });
}

void Systems::Delete(system::ISystem* pSystem)
{
    std::multiset<Entry>::const_iterator systemsIt = m_systems.begin();

    while (systemsIt != m_systems.end())
    {
        if (*systemsIt != pSystem)
        {
            ++systemsIt;
        }
        else
        {
            // per documentation std::set::erase does not invalidate other
            // pointers or references
            systemsIt = m_systems.erase(systemsIt);
        }
    }
}

void Systems::RunOnce(WorldTime::TimeUnit realDuration)
{
    m_pBuiltInSystems->time.Update(realDuration);

    for (Entry const& entry : m_systems)
    {
        entry->Update();
    }
}

} // namespace ecs
} // namespace sleipnir
