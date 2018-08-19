/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_TIMER_HPP
#define SLEIPNIR_SYSTEM_TIMER_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/entity/World.hpp>

#include <sleipnir/component/PositionComponent.hpp>
#include <sleipnir/component/TimerComponent.hpp>

#include <sleipnir/system/Skeleton.hpp>

namespace sleipnir
{
namespace system
{

/** @brief  System responsible for controlling timed events */
class Timer : public Skeleton<component::TimerComponent>
{
public:
    /** @brief  Basic constructor
     *
     *  @param  world       entity world
     *  @param  worldTime   time holder
     */
    Timer(entity::World& world, WorldTime& worldTime);

    //! Default destructor
    ~Timer() = default;

    /** @brief  Method invoked each loop cycle
     *
     *  Triggers active timers' @ref TimerComponent::onTick slots
     */
    void Update() override;

private:
    //! Time holder
    WorldTime& m_worldTime;
};

}
}

#endif // SLEIPNIR_SYSTEM_TIMER_HPP
