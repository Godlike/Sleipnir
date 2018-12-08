/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SLEIPNIR_ENGINE_HPP
#define SLEIPNIR_SLEIPNIR_ENGINE_HPP

#include <sleipnir/SleipnirConfigurator.hpp>

#include <sleipnir/ecs/Systems.hpp>
#include <sleipnir/ecs/WorldTime.hpp>
#include <sleipnir/ecs/entity/World.hpp>

#include <tulpar/TulparAudio.hpp>
#include <unicorn/UnicornRender.hpp>
#include <pegasus/Scene.hpp>

#include <cassert>

namespace sleipnir
{

/** @brief  Sleipnir library entry point */
class SleipnirEngine
{
public:
    /** @brief  Creates library instance
     *
     */
    SleipnirEngine();

    //! Disable copy constructor
    SleipnirEngine(SleipnirEngine const& other) = delete;

    //! Disable assignment operator
    SleipnirEngine& operator=(SleipnirEngine const& other) = delete;

    //! Deinitialize library instance
    ~SleipnirEngine();

    /** @brief  Initialize library instance
     *
     *  Using provided @p config initializes required subsystems
     *
     *  @param  config  sleipnir configuration
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    bool Initialize(SleipnirConfigurator const& config);

    /** @brief  Deinitializes all previously initialized subsystems */
    void Deinitialize();

    /** @brief  Initializes all created built-in systems */
    void InitializeBuiltInSystems();

    /** @brief  Starts control flow */
    void Run();

    /** @brief  Returns a reference to audio library */
    tulpar::TulparAudio& GetAudio() { assert(nullptr != m_tulparAudio); return *m_tulparAudio; }

    /** @brief  Returns a reference to rendering library */
    unicorn::UnicornRender& GetRender() { assert(nullptr != m_unicornRender); return *m_unicornRender; }

    /** @brief  Returns a reference to physics library */
    pegasus::scene::Scene& GetPhysics() { assert(nullptr != m_pegasusPhysics); return *m_pegasusPhysics; }

    /** @brief  Returns a reference to ECS system controller */
    ecs::Systems& GetSystems() { return m_systems; }

    /** @brief  Returns a reference to ECS entity world controller */
    ecs::entity::World& GetEntityWorld() { return m_entityWorld; }

    /** @brief  Returns a reference to ECS world time holder */
    ecs::WorldTime& GetWorldTime() { return m_worldTime; }

private:
    ecs::entity::World m_entityWorld;
    ecs::WorldTime m_worldTime;

    //! ECS system collection
    ecs::Systems m_systems;
    std::vector< std::unique_ptr<ecs::system::ISystem> > m_createdSystems;

    tulpar::TulparAudio* m_tulparAudio;
    unicorn::UnicornRender* m_unicornRender;
    pegasus::scene::Scene* m_pegasusPhysics;

};

}

#endif // SLEIPNIR_SLEIPNIR_ENGINE_HPP
