/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/system/physics/BodyController.hpp>
#include <sleipnir/system/physics/BodyHandle.hpp>
#include <sleipnir/system/physics/DynamicForceController.hpp>
#include <sleipnir/system/physics/PegasusAdapter.hpp>
#include <sleipnir/system/physics/SpawnInfo.hpp>

#include <sleipnir/utility/QSBR.hpp>

#include <atomic>
#include <thread>
#include <unordered_map>

namespace sleipnir
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

    /** @brief  Creates a body handle and order to spawn a body
     *
     *  Enqueues body creation in physics engine at current world time
     *
     *  @param  info    body information
     *
     *  @return newly created body handle
     *
     *  @sa PushBody(), DeleteBody()
     */
    BodyHandle* SpawnBody(SpawnInfo const& info);

    /** @brief  Applies given @p force to given @p bodyHandle
     *
     *  Enqueues body push in physics engine at current world time
     *
     *  @param  pHandle body handle
     *  @param  force   force to be applied
     *
     *  @sa SpawnBody(), DeleteBody()
     */
    void PushBody(BodyHandle const* pHandle, glm::vec3 force);

    /** @brief  Removes body handle from physics engine
     *
     *  Enqueues body deletion from physics engine at current world time
     *
     *  @param  bodyHandle  body handle
     *
     *  @sa SpawnBody(), PushBody()
     */
    void DeleteBody(BodyHandle const* pHandle);

    /** @brief  Creates gravity source in physics engine
     *
     *  @param  id          force id
     *  @param  position    gravity source position in world
     *  @param  magnitude   gravity pull strength
     *
     *  @sa DeleteGravitySource()
     */
    void CreateGravitySource(uint32_t id, glm::vec3 position, double magnitude);

    /** @brief  Deletes gravity source from physics engine
     *
     *  @param  id  force id
     *
     *  @sa CreateGravitySource()
     */
    void DeleteGravitySource(uint32_t id);

    //! Returns current time in physics world
    WorldTime::TimeUnit GetCurrentTime() const;

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
    PegasusAdapter m_physicsEngine;

    //! Force controller
    DynamicForceController m_dynamicForceController;

    //! Body controller
    BodyController m_bodyController;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_PHYSICS_THREAD_HPP
