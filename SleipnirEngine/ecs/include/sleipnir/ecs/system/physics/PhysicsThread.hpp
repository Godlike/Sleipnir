/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <sleipnir/ecs/system/physics/BodyHandle.hpp>
#include <sleipnir/ecs/system/physics/Engine.hpp>

#include <sleipnir/utility/QSBR.hpp>

#include <atomic>
#include <thread>
#include <unordered_map>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

/** @brief  Subsystem running physics engine in a dedicated thread
 *
 *  Responsible for controlling and communicating with physics engine
 */
class PhysicsThread
{
public:
    //! Shortcut to a collection of body positions
    using BodyPositions = std::unordered_map<pegasus::scene::Handle, glm::dvec3>;

    //! QSBR section id for @ref memoryReclaimer
    using SectionIndex = utility::QSBR::SectionIndex;

    /** @brief  Basic constructor
     *
     *  @param  worldTime   time holder
     */
    PhysicsThread(WorldTime& worldTime);

    //! Basic destructor
    ~PhysicsThread();

    //! Initializes physics engine
    void Initialize();

    //! Starts physics thread
    void Run();

    //! Signals physics thread to stop and waits until physics thread is finished
    void Join();

    //! Returns latest state of QSBR controlled body positions
    BodyPositions* GetBodyPositions() const;

    //! Returns current time in physics world
    WorldTime::TimeUnit GetCurrentTime() const;

    /** @brief  Creates change control instance linked to @p m_physicsEngine
     *
     *  @note   This method is a proxy for Engine::CloneBodyChanges()
     *
     *  @param  priority    priority for instance pushes
     *
     *  @return change control instance linked to @p m_physicsEngine
     */
    BodyChanges::Instance CloneBodyChanges(uint16_t priority = 0x8000) const
    {
        return m_physicsEngine.CloneBodyChanges(priority);
    }

    //! QSBR memory reclaimer controlling @ref BodyPositions
    utility::QSBR memoryReclaimer;

private:
    //! Structure holding time information
    struct TimeControl
    {
        //! Shortcut to time unit
        using TimeUnit = WorldTime::TimeUnit;

        /** @brief  Basic constructor
         *
         *  @param  worldTime   time holder
         */
        TimeControl(WorldTime& worldTime);

        //! Reference to time holder
        WorldTime& worldTime;

        //! Current physics world time in @ref TimeUnit
        TimeUnit currentTime;

        //! Current physics world time in @ref TimeUnit.count()
        std::atomic<uint64_t> currentTimeRaw;
    };

    //! Physics thread routine loop
    void Routine();

    /** @brief  Gathers current body positions
     *
     *  Is called for each physics iteration from Routine()
     *  Stores new body positions in @ref memoryReclaimer
     *  Marks previous body positions to be reclaimed when unused
     */
    void PollPositions();

    //! QSBR section id for physics subsystem
    SectionIndex m_sectionId;

    //! Physics thread
    std::thread m_thread;

    //! Flag indicating physics thread running state
    std::atomic_bool m_working;

    //! Time information
    TimeControl m_timeControl;

    //! Latest QSBR controlled body positions
    std::atomic<BodyPositions*> m_currentPositions;

    //! Physics engine adapter
    Engine m_physicsEngine;

};

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP
