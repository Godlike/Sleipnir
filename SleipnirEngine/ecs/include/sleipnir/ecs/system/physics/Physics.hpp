/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/entity/World.hpp>

#include <sleipnir/ecs/component/Flags.hpp>

#include <sleipnir/ecs/component/ControlComponent.hpp>
#include <sleipnir/ecs/component/PositionComponent.hpp>
#include <sleipnir/ecs/component/PhysicsComponent.hpp>

#include <sleipnir/ecs/system/Skeleton.hpp>
#include <sleipnir/ecs/system/physics/PhysicsThread.hpp>

#include <memory>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

/** @brief  System responsible for syncing objects with physics */
class Physics : public Skeleton<component::PositionComponent, component::PhysicsComponent>
{
public:
    /** @brief  Basic constructor
     *
     *  @param  world       entity world
     *  @param  worldTime   time holder
     */
    Physics(entity::World& world, WorldTime& worldTime);

    /** @brief  Basic destructor
     *
     *  Joins physics thread
     */
    ~Physics();

    /** @brief  Initializes physics subsystem
     *
     *  Starts physics thread
     */
    void Initialize();

    /** @brief  Method invoked each loop cycle
     *
     *  Updates entities' positions
     */
    void Update() override;

    /** @brief  Creates change control instance linked to @p m_physicsThread
     *
     *  @note   This method is a proxy for PhysicsThread::CloneBodyChanges()
     *
     *  @param  priority    priority for instance pushes
     *
     *  @return change control instance linked to @p m_physicsThread
     */
    BodyChanges::Instance CloneBodyChanges(uint16_t priority = 0x8000) const
    {
        return m_physicsThread.CloneBodyChanges(priority);
    }

    //! Returns current time in physics world
    WorldTime::TimeUnit GetCurrentTime() const
    {
        return m_physicsThread.GetCurrentTime();
    }

private:
    /** @brief  Subsystem responsible for handling physics controls */
    class Control : public Skeleton<component::PhysicsComponent, component::ControlComponent>
    {
    public:
        /** @brief  Basic constructor
         *
         *  @param  world           entity world
         *  @param  physicsThread   physics subsystem
         */
        Control(entity::World& world, WorldTime& worldTime, PhysicsThread& physicsThread);

        //! Default destructor
        ~Control() = default;

        /** @brief  Method invoked each loop cycle
         *
         *  Applies physics contols to the objects
         */
        void Update() override;

    private:
        //! Reference to time holder
        WorldTime& m_worldTime;

        //! Local instance of physics body changes
        BodyChanges::Instance m_physicsBodyChanges;
    };

    //! Physics subsystem
    PhysicsThread m_physicsThread;

    //! QSBR section id for physics subsystem
    PhysicsThread::SectionIndex m_sectionId;

    //! Physics control subsystem
    Control m_control;
};

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_HPP
