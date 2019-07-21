/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/PhysicsThread.hpp>

#include <sleipnir/ecs/system/physics/Body.hpp>

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
    , m_physicsEngine()
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

WorldTime::TimeUnit PhysicsThread::GetCurrentTime() const
{
    return WorldTime::TimeUnit(m_timeControl.currentTimeRaw.load(std::memory_order_acquire));
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
                mule::ScopeProfiler profiler(LOG_PROFILE, "pegasus_run");
                m_physicsEngine.Run(utility::Config::PhysicsTick);
            }

            m_timeControl.currentTime = future;

            PollPositions();

            m_timeControl.currentTimeRaw.store(future.count(), std::memory_order_release);

            memoryReclaimer.OnQuiescentState(m_sectionId);

            {
                mule::ScopeProfiler profiler(LOG_PROFILE, "pegasus_integrate");
                m_physicsEngine.Integrate(future);
            }
        }

        std::this_thread::yield();
    } while (m_working.load(std::memory_order_acquire));
}

void PhysicsThread::PollPositions()
{
    BodyPositions* pOldPositions = m_currentPositions.load(std::memory_order_acquire);
    BodyPositions* pNewPositions = new BodyPositions();

    {
        BodyCollection const& bodies = m_physicsEngine.GetBodyCollection();
        BodyCollection::Collection const& objects = bodies.GetObjectCollection();

        BodyPositions& newPositions = *pNewPositions;
        newPositions.reserve(objects.size() + 1);

        newPositions[BodyObject::UNKNOWN_ID] = glm::dvec3{
            std::numeric_limits<double>::quiet_NaN()
            , std::numeric_limits<double>::quiet_NaN()
            , std::numeric_limits<double>::quiet_NaN()
        };

        for (auto const& [handle, pObject] : objects)
        {
            newPositions[handle] = pObject->GetPosition();
        }
    }

    m_currentPositions.store(pNewPositions, std::memory_order_release);

    memoryReclaimer.AddCallback([=](){ delete pOldPositions; });
}

} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir
