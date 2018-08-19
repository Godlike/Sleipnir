/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_WORLD_TIME_HPP
#define SLEIPNIR_WORLD_TIME_HPP

#include <atomic>
#include <chrono>
#include <cstdint>

namespace sleipnir
{

/** @brief  Holds time point
 *
 *  Constitutes a time flow. Governing object decides how the time shall flow
 *
 *  This class uses atomic container for stored time point so it can be safely
 *  used in multithreaded environment
 */
class WorldTime
{
public:
    //! Shortcut to underlying time unit
    using TimeUnit = std::chrono::microseconds;

    //! Basic constructor
    WorldTime() : m_time(0) {}

    //! Default destructor
    ~WorldTime() = default;

    /** @brief  Set time point
     *
     *  @param  time    time point to be set
     */
    void SetTime(TimeUnit time) { m_time.store(time.count()); }

    /** @brief  Get time point
     *
     *  @return stored time point
     */
    TimeUnit GetTime() const { return TimeUnit(m_time.load()); }

private:
    //! Holds time point offset
    std::atomic<uint64_t> m_time;
};

}

#endif // SLEIPNIR_WORLD_TIME_HPP
