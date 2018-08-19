/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_SKELETON_HPP
#define SLEIPNIR_SYSTEM_SKELETON_HPP

#include <sleipnir/entity/EntityPool.hpp>
#include <sleipnir/entity/World.hpp>

#include <sleipnir/component/Flags.hpp>

#include <sleipnir/system/ISystem.hpp>

namespace sleipnir
{
namespace system
{

/** @brief  System template class
 *
 *  Generates GetEntities() method to get entities that have
 *  all of @p Comps components
 *
 *  @tparam Comps   required components
 */
template<class... Comps>
    class Skeleton : public ISystem
{
public:
    /** @brief  Constructs system skeleton
     *
     *  @param  world   entity world
     */
    Skeleton(entity::World& world);

    //! Basic destructor
    ~Skeleton();

protected:
    /** @brief  Returns entities that have @p Comps
     *
     *  @return collection of matching entities
     */
    entity::World::Entities GetEntities();

    //! Entity world
    entity::World& m_world;

private:
    //! Component matching query
    component::Flags m_query;
};

}
}

#include <sleipnir/system/Skeleton.imp>

#endif // SLEIPNIR_SYSTEM_SKELETON_HPP
