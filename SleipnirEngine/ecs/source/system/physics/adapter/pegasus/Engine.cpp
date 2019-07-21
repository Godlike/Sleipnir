/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/ecs/system/physics/adapter/pegasus/Engine.hpp>

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

Engine::Engine()
    : m_bodyCollection(m_scene)
    , m_bodyIntegrator(m_bodyCollection)
{

}

void Engine::Initialize()
{

}

void Engine::Run(WorldTime::TimeUnit tick)
{
    m_scene.ComputeFrame(
        static_cast<float>(tick.count())
        * (static_cast<float>(decltype(tick)::period::num)
            / static_cast<float>(decltype(tick)::period::den)
        )
    );
}

void Engine::Integrate(WorldTime::TimeUnit timepoint)
{
    BodyChanges::Instance diff = m_bodyChanges.Pull(timepoint);
    m_bodyIntegrator.Integrate(diff);
}

}
}
}
}
}
}