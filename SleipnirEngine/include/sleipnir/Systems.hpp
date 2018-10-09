/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEMS_HPP
#define SLEIPNIR_SYSTEMS_HPP

#include <sleipnir/ISystem.hpp>

#include <cstdint>
#include <set>

namespace sleipnir
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
        Input       = 0x1000
        , Time      = 0x2000
        , Lifetime  = 0x3000
        , Timer     = 0x4000
        , Physics   = 0x5000
        , Render    = 0x6000
    };

    /** @brief  Basic constructor
     *
     *  Registers default systems
     */
    Systems();

    //! Default destructor
    ~Systems() = default;

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
     */
    void RunOnce();

private:
    //! System-priority coupling
    struct Entry
    {
        //! Priority
        uint16_t priority;

        //! Pointer to system
        system::ISystem* pSystem;

        bool operator<(Entry const& rhs) const
        {
            return priority < rhs.priority;
        }

        bool operator==(system::ISystem* pSystem) const
        {
            return this->pSystem == pSystem;
        }
    };

    std::set<Entry> m_systems;
};

}

#endif // SLEIPNIR_SYSTEMS_HPP
