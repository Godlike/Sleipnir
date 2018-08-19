/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_LIFETIME_HPP
#define SLEIPNIR_SYSTEM_LIFETIME_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/entity/World.hpp>

#include <sleipnir/component/LifetimeComponent.hpp>

#include <sleipnir/system/Skeleton.hpp>

namespace sleipnir
{

namespace system
{

/** @brief  System responsible for controlling objects' lifetime */
class Lifetime : public Skeleton<component::LifetimeComponent>
{
public:
    //! Shortcut to user's entity reclaiming callback
    using EntityReclaimer = std::function<void(const Entity&)>;

    /** @brief  Basic constructor
     *
     *  @param  world       entity world
     *  @param  worldTime   time holder
     *  @param  reclaimer   entity reclaiming callback
     */
    Lifetime(entity::World& world, WorldTime& worldTime, EntityReclaimer reclaimer);

    //! Default destructor
    ~Lifetime() = default;

    /** @brief  Method invoked each loop cycle
     *
     *  Compares objects' lifetime with current time and destroys expired entities
     */
    void Update() override;

private:
    //! Time holder
    WorldTime& m_worldTime;

    //! Entity reclaiming callback called whenever an expired entity is destroyed
    EntityReclaimer m_reclaimer;
};

}
}

#endif // SLEIPNIR_SYSTEM_LIFETIME_HPP
