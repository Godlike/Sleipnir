/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEMS_HPP
#define SLEIPNIR_ECS_SYSTEMS_HPP

#include <sleipnir/ecs/system/ISystem.hpp>
#include <sleipnir/ecs/system/TimeBase.hpp>

#include <sleipnir/ecs/entity/World.hpp>
#include <sleipnir/ecs/WorldTime.hpp>

#include <cassert>
#include <cstdint>
#include <memory>
#include <set>

namespace sleipnir
{
namespace ecs
{

namespace system
{
namespace physics
{
    class Physics;
}

class Render;

}

/** @brief  Runs registered systems
 *
 *  Uses priority to sort the systems
 */
class Systems final
{
public:
    //! Bujlt-in priorities for default systems
    struct DefaultPriority
    {
        static constexpr uint16_t Input     = 0x1000;
        static constexpr uint16_t Lifetime  = 0x2000;
        static constexpr uint16_t Timer     = 0x3000;
        static constexpr uint16_t Physics   = 0x4000;
        static constexpr uint16_t Render    = 0x5000;
        static constexpr uint16_t Audio     = 0x6000;
    };

    /** @brief  Basic constructor
     *
     *  Registers default time systems
     *
     *  @param  worldTime   time holder
     */
    Systems(WorldTime& worldTime);

    //! Default destructor
    ~Systems() = default;

    /** @brief  Set time system
     *
     *  @param  timeSystem  smart poitner to a time system
     */
    void SetTimeSystem(std::shared_ptr<system::TimeBase> timeSystem);

    /** @brief  Add system to the pool
     *
     *  @param  pSystem     pointer to system
     *  @param  priority    system priority
     */
    void Add(system::ISystem* pSystem, uint16_t priority);

    /** @brief  Deletes system from the pool
     *
     *  @param  pSystem pointer to system
     */
    void Delete(system::ISystem* pSystem);

    /** @brief  Run all registered systems
     *
     *  Iterates over systems stored in pool and executes them
     *
     *  @param  realDuration    raw duration of current frame
     */
    void RunOnce(WorldTime::TimeUnit realDuration);

    /** @brief  Sets render system
     *
     *  Also adds given system to @ref m_systems
     *
     *  @param  system  reference to render system
     *
     *  @sa Add
     */
    void SetRender(system::Render& system);

    /** @brief  Returns render system */
    system::Render& GetRender() const { assert(nullptr != m_pRenderSystem); return *m_pRenderSystem; }

    /** @brief  Sets physics system
     *
     *  Also adds given system to @ref m_systems
     *
     *  @param  system  reference to physics system
     *
     *  @sa Add
     */
    void SetPhysics(system::physics::Physics& system);

    /** @brief  Returns physics system */
    system::physics::Physics& GetPhysics() const { assert(nullptr != m_pPhysicsSystem); return *m_pPhysicsSystem; }

    /** @brief  Returns time system */
    system::TimeBase& GetTime() const { assert(nullptr != m_timeSystem); return *m_timeSystem.get(); }

private:
    //! System-priority coupling
    struct Entry
    {
        //! Priority
        uint16_t priority;

        //! Pointer to system
        system::ISystem* pSystem;

        system::ISystem* operator->() const
        {
            return pSystem;
        }

        friend bool operator<(Entry const& lhs, Entry const& rhs)
        {
            return (lhs.priority < rhs.priority) ? true
                : ((lhs.priority > rhs.priority) ? false
                    : (lhs.pSystem < rhs.pSystem));
        }

        friend bool operator==(Entry const& lhs, system::ISystem* pSystem)
        {
            return lhs.pSystem == pSystem;
        }

        friend bool operator!=(Entry const& lhs, system::ISystem* pSystem)
        {
            return !operator==(lhs, pSystem);
        }
    };

    system::Render* m_pRenderSystem;
    system::physics::Physics* m_pPhysicsSystem;

    std::shared_ptr<system::TimeBase> m_timeSystem;
    std::set<Entry> m_systems;
};

} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEMS_HPP
