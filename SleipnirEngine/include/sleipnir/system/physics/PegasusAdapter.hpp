/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_PEGASUS_ADAPTER_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_PEGASUS_ADAPTER_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/system/physics/SpawnInfo.hpp>

#include <pegasus/Scene.hpp>
#include <pegasus/Primitives.hpp>
#include <pegasus/Force.hpp>

#include <cstddef>
#include <list>
#include <map>
#include <vector>

namespace sleipnir
{
namespace system
{
namespace physics
{

/** @brief  Adapter for Pegasus API
 *
 *  Exposes operations usable through Sleipnir
 */
class PegasusAdapter
{
public:
    //! Shortcut to a collection of pegasus::scene::Primitive pointers
    using Primitives = std::list<pegasus::scene::Primitive*>;

    //! Basic constructor
    PegasusAdapter();

    //! Default destructor
    ~PegasusAdapter() = default;

    //! Initialize default pegasus state
    void Initialize();

    /** @brief  Spawns physics body handle
     *
     *  @param  info    body information
     *
     *  @return newly created body handle
     *
     *  @sa PushBody(), DeleteBody()
     */
    pegasus::scene::Handle SpawnBody(SpawnInfo const& info);

    /** @brief  Applies given @p force to given @p bodyHandle
     *
     *  @param  bodyHandle  body handle
     *  @param  force       force to be applied
     *
     *  @sa SpawnBody(), DeleteBody()
     */
    void PushBody(pegasus::scene::Handle bodyHandle, glm::vec3 force);

    /** @brief  Removes body handle from pegasus
     *
     *  @param  bodyHandle  body handle
     *
     *  @sa SpawnBody(), PushBody()
     */
    void DeleteBody(pegasus::scene::Handle bodyHandle);

    /** @brief  Creates gravity source in pegasus
     *
     *  @param  id          force id
     *  @param  position    gravity source position in world
     *  @param  magnitude   gravity pull strength
     *
     *  @sa DeleteGravitySource()
     */
    void CreateGravitySource(uint32_t id, glm::vec3 position, double magnitude);

    /** @brief  Deletes gravity source from pegasus
     *
     *  @param  id  force id
     *
     *  @sa CreateGravitySource()
     */
    void DeleteGravitySource(uint32_t id);

    /** @brief  Progress physics world by given time period
     *
     *  @param  tick    time period
     */
    void Run(WorldTime::TimeUnit tick);

    //! Collection of primitives
    Primitives primitives;

    //! Amount of primitives in @p primitives
    std::size_t primitiveCount;

private:
    //! Shortcut to a collection of static forces
    using Forces = std::vector<std::unique_ptr<pegasus::scene::Force<pegasus::force::StaticField>>>;

    //! Shortcut to a collection of gravity sources
    using GravitySourceMap = std::unordered_map<uint32_t, std::unique_ptr<pegasus::scene::Force<pegasus::force::SquareDistanceSource>>>;

    //! Pegasus world
    pegasus::scene::Scene m_scene;

    //! Collection of static forces
    Forces m_staticForces;

    //! Collection of gravity sources
    GravitySourceMap m_dynamicForces;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_PEGASUS_ADAPTER_HPP
