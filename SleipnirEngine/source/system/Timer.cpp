/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/system/Timer.hpp>

namespace sleipnir
{
namespace system
{

Timer::Timer(entity::World& world
    , WorldTime& worldTime
)
    : Skeleton<component::TimerComponent>(world)
    , m_worldTime(worldTime)
{

}

void Timer::Update()
{
    WorldTime::TimeUnit const now = m_worldTime.GetTime();

    entity::World::Entities entities = GetEntities();

    for (entity::Entity& entity : entities)
    {
        component::TimerComponent& timerComp = entity.GetComponent<component::TimerComponent>();

        while (now >= (timerComp.lastTime + timerComp.tick))
        {
            timerComp.onTick.emit(entity);
            timerComp.lastTime += timerComp.tick;

            if ((timerComp.lastTime + timerComp.tick) >= timerComp.endTime)
            {
                break;
            }
        }

        if (now >= timerComp.endTime)
        {
            timerComp.onTimeout.emit(entity);
            entity.DeleteComponent<component::TimerComponent>();
            continue;
        }
    }
}

}
}
