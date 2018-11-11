/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEMS_HPP
#define SLEIPNIR_ECS_SYSTEMS_HPP

#include <sleipnir/ecs/system/ISystem.hpp>

#include <sleipnir/ecs/entity/World.hpp>
#include <sleipnir/ecs/WorldTime.hpp>

#include <cstdint>
#include <set>

namespace sleipnir
{
namespace ecs
{

/** @brief  Runs registered systems
 *
 *  Uses priority to sort the systems
 */
class Systems final
{
public:
    //! Bujlt-in priorities for default systems
    enum class DefaultPriority : uint16_t
    {
        Time        = 0x0000
        , Input     = 0x1000
        , Lifetime  = 0x2000
        , Timer     = 0x3000
        , Physics   = 0x4000
        , Render    = 0x5000
    };

    /** @brief  Basic constructor
     *
     *  Registers default systems
     *
     *  @param  world       entity world
     *  @param  worldTime   time holder
     */
    Systems(entity::World& world, WorldTime& worldTime);

    //! Basic destructor
    ~Systems();

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
            return lhs.priority < rhs.priority;
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

    struct BuiltInSystems;

    BuiltInSystems* m_pBuiltInSystems;

    std::set<Entry> m_systems;
};

} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEMS_HPP
