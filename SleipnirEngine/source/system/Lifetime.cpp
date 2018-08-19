/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/system/Lifetime.hpp>

#include <sleipnir/entity/Entity.hpp>

#include <iostream>

namespace sleipnir
{
namespace system
{

Lifetime::Lifetime(entity::World& world
    , WorldTime& worldTime
    , EntityReclaimer reclaimer
)
    : Skeleton<component::LifetimeComponent>(world)
    , m_worldTime(worldTime)
    , m_reclaimer(reclaimer)
{

}

void Lifetime::Update()
{
    using component::LifetimeComponent;

    entity::World::Entities entities = GetEntities();
    WorldTime::TimeUnit const now = m_worldTime.GetTime();

    uint32_t count = 0;

    for (entity::Entity& entity : entities)
    {
        LifetimeComponent& component = entity.GetComponent<LifetimeComponent>();

        if (now >= component.deadline)
        {
            m_reclaimer(entity);
            m_world.DeleteEntity(entity);
            ++count;
        }
    }

    if (count)
    {
        std::cerr << "[Lifetime] deleted " << count << " entities" << std::endl;
    }
}

}
}
