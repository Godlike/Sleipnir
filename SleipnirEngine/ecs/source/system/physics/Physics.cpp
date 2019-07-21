/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/Physics.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

// Physics::

Physics::Physics(entity::World& world, WorldTime& worldTime)
    : Skeleton<component::PositionComponent, component::PhysicsComponent>(world)
    , m_physicsThread(worldTime)
    , m_sectionId(m_physicsThread.memoryReclaimer.RegisterSection())
    , m_control(world, worldTime, m_physicsThread)
{

}

Physics::~Physics()
{
    m_physicsThread.Join();
}

void Physics::Initialize()
{
    m_physicsThread.Initialize();
    m_physicsThread.Run();
}

void Physics::Update()
{
    m_control.Update();

    entity::World::Entities entities = GetEntities();
    PhysicsThread::BodyPositions const& positions = *m_physicsThread.GetBodyPositions();

    for (entity::Entity& entity : entities)
    {
        component::PositionComponent& posComp = entity.GetComponent<component::PositionComponent>();
        component::PhysicsComponent const& physComp = entity.GetComponent<component::PhysicsComponent>();

        PhysicsThread::BodyPositions::const_iterator cit = positions.find(physComp.pHandle->bodyHandle.load());

        if (positions.cend() != cit)
        {
            posComp.position = static_cast<glm::vec3>(cit->second);
        }
    }

    m_physicsThread.memoryReclaimer.OnQuiescentState(m_sectionId);
}

// Physics::Control::

Physics::Control::Control(entity::World& world, WorldTime& worldTime, PhysicsThread& physicsThread)
    : Skeleton<component::PhysicsComponent, component::ControlComponent>(world)
    , m_worldTime(worldTime)
    , m_physicsBodyChanges(physicsThread.CloneBodyChanges())
{

}

void Physics::Control::Update()
{
    entity::World::Entities entities = GetEntities();

    for (entity::Entity& entity : entities)
    {
        component::PhysicsComponent const& physComp = entity.GetComponent<component::PhysicsComponent>();
        component::ControlComponent const& controlComp = entity.GetComponent<component::ControlComponent>();

        BodyMemento::LinearMotion linear;
        linear.force = controlComp.force;

        BodyMemento memento;
        memento.handle = physComp.pHandle->bodyHandle.load();
        memento.linear = { true, linear };

        m_physicsBodyChanges.Modify(physComp.pHandle, memento, &BodyObject::operator+=);

        entity.DeleteComponent<component::ControlComponent>();
    }

    m_physicsBodyChanges.Push(m_worldTime.GetTime());
}

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir
