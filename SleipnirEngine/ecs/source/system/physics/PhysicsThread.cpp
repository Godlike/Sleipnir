/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/PhysicsThread.hpp>

#include <sleipnir/utility/InternalLoggers.hpp>

#include <sleipnir/utility/Config.hpp>

#include <mule/ScopeProfiler.hpp>

#include <limits>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

// PhysicsThread

PhysicsThread::PhysicsThread(WorldTime& worldTime)
    : m_sectionId(memoryReclaimer.RegisterSection())
    , m_working(true)
    , m_timeControl(worldTime)
    , m_currentPositions(nullptr)
    , m_physicsEngine(m_changeControl)
    , m_dynamicForceController(m_physicsEngine)
    , m_changeControl()
{
    m_currentPositions.store(new BodyPositions(), std::memory_order_release);
}

PhysicsThread::~PhysicsThread()
{
    delete m_currentPositions.load(std::memory_order_acquire);
}

void PhysicsThread::Initialize()
{
    m_physicsEngine.Initialize();
}

void PhysicsThread::Run()
{
    m_thread = std::thread(std::bind(&PhysicsThread::Routine, this));
}

void PhysicsThread::Join()
{
    m_working.store(false, std::memory_order_release);

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

PhysicsThread::BodyPositions* PhysicsThread::GetBodyPositions() const
{
    return m_currentPositions.load(std::memory_order_acquire);
}

void PhysicsThread::CreateGravitySource(uint32_t id, glm::vec3 position, double magnitude)
{
    m_dynamicForceController.Create({
        id
        , m_timeControl.worldTime.GetTime()
        , position
        , magnitude
    });
}

void PhysicsThread::DeleteGravitySource(uint32_t id)
{
    m_dynamicForceController.Delete({
        id
        , m_timeControl.worldTime.GetTime()
    });
}

WorldTime::TimeUnit PhysicsThread::GetCurrentTime() const
{
    return WorldTime::TimeUnit(m_timeControl.currentTimeRaw.load(std::memory_order_acquire));
}

ChangeControl::Instance PhysicsThread::CloneChanges(uint16_t priority)
{
    return m_changeControl.Clone(priority);
}

// PhysicsThread::TimeControl

PhysicsThread::TimeControl::TimeControl(WorldTime& worldTime)
    : worldTime(worldTime)
    , currentTime(worldTime.GetTime())
    , currentTimeRaw(currentTime.count())
{

}

// PhysicsThread

void PhysicsThread::Routine()
{
    TimeControl::TimeUnit target;

    do
    {
        target = m_timeControl.worldTime.GetTime();

        for (TimeControl::TimeUnit future = (m_timeControl.currentTime + utility::Config::PhysicsTick); future < target; future += utility::Config::PhysicsTick)
        {
            {
                mule::ScopeProfiler profiler(LOG_PROFILE, "pegasus");
                m_physicsEngine.Run(utility::Config::PhysicsTick);
            }

            m_timeControl.currentTime = future;

            PollPositions();

            m_timeControl.currentTimeRaw.store(future.count(), std::memory_order_release);

            memoryReclaimer.OnQuiescentState(m_sectionId);

            m_dynamicForceController.Check(future);
            m_bodyController.Check(future);
        }

        std::this_thread::yield();
    } while (m_working.load(std::memory_order_acquire));
}

void PhysicsThread::PollPositions()
{
    BodyPositions* pOldPositions = m_currentPositions.load(std::memory_order_acquire);
    BodyPositions* pNewPositions = new BodyPositions();

    {
        BodyPositions& newPositions = *pNewPositions;
        newPositions.reserve(m_physicsEngine.primitiveCount + 1);

        newPositions[pegasus::scene::Handle()] = glm::dvec3{
            std::numeric_limits<double>::quiet_NaN()
            , std::numeric_limits<double>::quiet_NaN()
            , std::numeric_limits<double>::quiet_NaN()
        };

        for (pegasus::scene::Primitive const* const primitive : m_physicsEngine.primitives)
        {
            newPositions[primitive->GetBodyHandle()] = primitive->GetBody().linearMotion.position;
        }
    }

    m_currentPositions.store(pNewPositions, std::memory_order_release);

    memoryReclaimer.AddCallback([=](){ delete pOldPositions; });
}

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir
