#ifndef SLEIPNIR_APPLICATION_HPP
#define SLEIPNIR_APPLICATION_HPP

#include <unicorn/utility/Settings.hpp>
#include <unicorn/UnicornRender.hpp>

#include <unicorn/system/Timer.hpp>
#include <unicorn/system/Window.hpp>

#include <unicorn/video/CameraFpsController.hpp>
#include <unicorn/video/geometry/MeshDescriptor.hpp>
#include <unicorn/video/Renderer.hpp>

#include <pegasus/Particle.hpp>
#include <pegasus/ParticleContacts.hpp>
#include <pegasus/ParticleForceGenerator.hpp>
#include <pegasus/ParticleWorld.hpp>

#include <list>

class Application
{
public:
    Application(unicorn::utility::Settings& settings
        , unicorn::UnicornRender* pRender);
    ~Application();

    void Run();

private:
    static const uint32_t MAX_OBJECT_COUNT = 5;

    void OnLogicFrame(unicorn::UnicornRender* pRender);

    void OnMouseButton(unicorn::system::Window::MouseButtonEvent const& mouseButtonEvent);
    void OnCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos);
    void OnMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos);

    void OnWindowKeyboard(unicorn::system::Window::KeyboardEvent const& keyboardEvent);

    void OnRendererDestroyed(unicorn::video::Renderer* pRenderer);

    void SpawnCube();
    void DeleteCube();

    void SceneReset();

    float m_deltaTime;
    float m_lastFrame;

    // Render part
    using MeshDescriptor = unicorn::video::geometry::MeshDescriptor;

    unicorn::UnicornRender* m_pRender;
    unicorn::system::Timer m_timer;

    unicorn::video::CameraFpsController* m_pCameraController;
    unicorn::video::Renderer* m_pVkRenderer;

    std::list<MeshDescriptor*> m_cubes;
    std::list<MeshDescriptor*> m_worldObjects;

    // Physics part
    using Particles = std::list<pegasus::Particle>;
    using RigidBodies = std::list<pegasus::RigidBody>;
    using ForceGenerators = std::list<std::unique_ptr<pegasus::ParticleForceGenerator>>;

    pegasus::ParticleForceRegistry m_physicsForceRegistry;
    pegasus::ParticleContactGenerators m_physicsContactGenerators;
    pegasus::ParticleWorld m_physicsWorld;

    Particles m_particles;
    ForceGenerators m_forces;
    RigidBodies m_rigidBodies;

    // Glue
    std::unordered_map<pegasus::Particle*, MeshDescriptor*> m_glue;
};

#endif // SLEIPNIR_APPLICATION_HPP
