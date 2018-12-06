/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/SleipnirEngine.hpp>

#include <sleipnir/ecs/system/physics/Physics.hpp>
#include <sleipnir/ecs/system/Render.hpp>
#include <sleipnir/ecs/system/TimeWithPhysics.hpp>

#include <mule/Loggers.hpp>
#include <mule/MuleUtilities.hpp>

namespace sleipnir
{

SleipnirEngine::SleipnirEngine()
    : m_systems(m_worldTime)
    , m_tulparAudio(nullptr)
    , m_unicornRender(nullptr)
{

}

SleipnirEngine::~SleipnirEngine()
{
    Deinitialize();
}

bool SleipnirEngine::Initialize(SleipnirConfigurator const& config)
{
    bool success = true;

    mule::Loggers::Instance().SetDefaultSettings(config.globalLoggerSettings);

    mule::MuleUtilities::Initialize();

    { //! Tulpar initialization
        SleipnirConfigurator::Tulpar const& tulpar = config.tulpar;

        if (nullptr != tulpar.config)
        {
            tulpar::Loggers::Settings const& loggerSettings = (nullptr == tulpar.loggerOverride)
                ? config.globalLoggerSettings
                : *(tulpar.loggerOverride)
            ;

            tulpar::Loggers::Instance().SetDefaultSettings(loggerSettings);

            m_tulparAudio = new tulpar::TulparAudio();
            success &= m_tulparAudio->Initialize(*tulpar.config);
        }
    }

    { //! Unicorn initialization
        SleipnirConfigurator::Unicorn const& unicorn = config.unicorn;

        if (nullptr != unicorn.config)
        {
            unicorn::Loggers::Settings const& loggerSettings = (nullptr == unicorn.loggerOverride)
                ? config.globalLoggerSettings
                : *(unicorn.loggerOverride)
            ;

            unicorn::Loggers::Instance().SetDefaultSettings(loggerSettings);

            m_unicornRender = new unicorn::UnicornRender();
            success &= m_unicornRender->Init();

            ecs::system::ISystem* pSystem = new ecs::system::Render(m_entityWorld);
            m_createdSystems.emplace_back(pSystem);
            m_systems.Add(pSystem, static_cast<uint16_t>(ecs::Systems::DefaultPriority::Render));
        }
    }

    { //! Pegasus initialization
        SleipnirConfigurator::Pegasus const& pegasus = config.pegasus;

        if (nullptr != pegasus.config)
        {
            // pegasus::Loggers::Settings const& loggerSettings = (nullptr == pegasus.loggerOverride)
            //     ? config.globalLoggerSettings
            //     : *(pegasus.loggerOverride)
            // ;

            // pegasus::Loggers::Instance().SetDefaultSettings(loggerSettings);

            m_pegasusPhysics = new pegasus::scene::Scene();

            ecs::system::physics::Physics* pSystem = new ecs::system::physics::Physics(m_entityWorld, m_worldTime);
            m_createdSystems.emplace_back(pSystem);
            m_systems.Add(pSystem, static_cast<uint16_t>(ecs::Systems::DefaultPriority::Physics));

            m_systems.SetTimeSystem(std::make_shared<ecs::system::TimeWithPhysics>(m_worldTime, *pSystem));
        }
    }

    { //! Lifetime initialization
        SleipnirConfigurator::Lifetime const& lifetime = config.lifetime;

        ecs::system::ISystem* pSystem = new ecs::system::Lifetime(m_entityWorld, m_worldTime, lifetime.reclaimer);
        m_systems.Add(pSystem, static_cast<uint16_t>(ecs::Systems::DefaultPriority::Lifetime));
    }

    return success;
}

void SleipnirEngine::Deinitialize()
{
    if (nullptr != m_pegasusPhysics)
    {
        delete m_pegasusPhysics;

        m_pegasusPhysics = nullptr;
    }

    if (nullptr != m_unicornRender)
    {
        m_unicornRender->Deinit();

        delete m_unicornRender;

        m_unicornRender = nullptr;
    }

    if (nullptr != m_tulparAudio)
    {
        m_tulparAudio->Deinitialize();

        delete m_tulparAudio;

        m_tulparAudio = nullptr;
    }
}

}
