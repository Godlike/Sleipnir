/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_ENGINE_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_ENGINE_HPP

#include <sleipnir/ecs/WorldTime.hpp>
#include <sleipnir/ecs/system/physics/Primitives.hpp>

#include <sleipnir/ecs/system/physics/adapter/pegasus/Body.hpp>

#include <pegasus/Scene.hpp>

#include <cstddef>
#include <list>
#include <map>
#include <vector>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{
namespace adapter
{
namespace pegasus
{

/** @brief  Adapter for Pegasus API
 *
 *  Exposes operations usable through Sleipnir
 */
class Engine
{
public:
    //! Basic constructor
    Engine();

    //! Default destructor
    ~Engine() = default;

    //! Initialize default pegasus state
    void Initialize();

    /** @brief  Progress physics world by given time period
     *
     *  @param  tick    time period
     */
    void Run(WorldTime::TimeUnit tick);

    /** @brief  Integrate changes up until given @p timepoint
     *
     *  @param  timepoint   target time point of integration
     */
    void Integrate(WorldTime::TimeUnit timepoint);

    /** @brief  Creates change control instance linked to @p m_bodyChanges
     *
     *  @param  priority    priority for instance pushes
     *
     *  @return change control instance linked to @p m_bodyChanges
     */
    BodyChanges::Instance CloneBodyChanges(uint16_t priority = 0x8000) const { return m_bodyChanges.Clone(priority); }

    //! Returns a const reference to physics body collection
    BodyCollection const& GetBodyCollection() const { return m_bodyCollection; }

private:
    //! Pegasus world
    ::pegasus::scene::Scene m_scene;

    BodyCollection m_bodyCollection;
    BodyChanges m_bodyChanges;
    BodyChanges::Integrator<BodyCollection> m_bodyIntegrator;

};

} // namespace pegasus
} // namespace adapter
} // namespace physics
} // namespace system
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_ENGINE_HPP
