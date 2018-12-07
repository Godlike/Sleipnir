/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/Systems.hpp>

#include <sleipnir/ecs/system/physics/Physics.hpp>
#include <sleipnir/ecs/system/Render.hpp>

namespace sleipnir
{
namespace ecs
{

Systems::Systems(WorldTime& worldTime)
    : m_pRenderSystem(nullptr)
    , m_pPhysicsSystem(nullptr)
    , m_timeSystem(std::make_shared<system::TimeBase>(worldTime))
{

}

void Systems::SetTimeSystem(std::shared_ptr<system::TimeBase> timeSystem)
{
    m_timeSystem = timeSystem;
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

    if (pSystem == static_cast<system::ISystem*>(m_pRenderSystem))
    {
        m_pRenderSystem = nullptr;
    }

    if (pSystem == static_cast<system::ISystem*>(m_pPhysicsSystem))
    {
        m_pPhysicsSystem = nullptr;
    }
}

void Systems::RunOnce(WorldTime::TimeUnit realDuration)
{
    m_timeSystem->Update(realDuration);

    for (Entry const& entry : m_systems)
    {
        entry->Update();
    }
}

void Systems::SetRender(system::Render& system)
{
    m_pRenderSystem = &system;

    Add(m_pRenderSystem, DefaultPriority::Render);
}

void Systems::SetPhysics(system::physics::Physics& system)
{
    m_pPhysicsSystem = &system;

    Add(m_pPhysicsSystem, DefaultPriority::Physics);
}

} // namespace ecs
} // namespace sleipnir
