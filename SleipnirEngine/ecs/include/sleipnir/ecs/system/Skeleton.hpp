/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_SKELETON_HPP
#define SLEIPNIR_ECS_SYSTEM_SKELETON_HPP

#include <sleipnir/ecs/entity/EntityPool.hpp>
#include <sleipnir/ecs/entity/World.hpp>

#include <sleipnir/ecs/component/Flags.hpp>

#include <sleipnir/ecs/system/ISystem.hpp>

namespace sleipnir
{
namespace ecs
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

} // namespace system
} // namespace ecs
} // namespace sleipnir

#include <sleipnir/ecs/system/Skeleton.imp>

#endif // SLEIPNIR_ECS_SYSTEM_SKELETON_HPP
