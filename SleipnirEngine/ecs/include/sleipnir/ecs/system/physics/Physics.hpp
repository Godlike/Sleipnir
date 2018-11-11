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
#include <sleipnir/ecs/system/physics/SpawnInfo.hpp>

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

    /** @brief  Spawns physics body handle
     *
     *  @param  info    body information
     *
     *  @return newly created body handle
     *
     *  @sa DeleteBody()
     */
    BodyHandle* SpawnBody(SpawnInfo const& info);

    /** @brief  Removes body handle from physics subsystem
     *
     *  @param  pHandle pointer to body handle
     *
     *  @sa SpawnBody()
     */
    void DeleteBody(BodyHandle const* pHandle);

    /** @brief  Creates gravity source in physics subsystem
     *
     *  @param  id          force id
     *  @param  position    gravity source position in world
     *  @param  magnitude   gravity pull strength
     *
     *  @sa DeleteGravitySource()
     */
    void CreateGravitySource(uint32_t id, glm::vec3 position, double magnitude);

    /** @brief  Deletes gravity source from physics sybsystem
     *
     *  @param  id  force id
     *
     *  @sa CreateGravitySource()
     */
    void DeleteGravitySource(uint32_t id);

    //! Returns current time in physics world
    WorldTime::TimeUnit GetCurrentTime() const { return m_physicsThread.GetCurrentTime(); }

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
        Control(entity::World& world, PhysicsThread& physicsThread);

        //! Default destructor
        ~Control() = default;

        /** @brief  Method invoked each loop cycle
         *
         *  Applies physics contols to the objects
         */
        void Update() override;

    private:
        //! Reference to physics subsystem
        PhysicsThread& m_physicsThread;
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
