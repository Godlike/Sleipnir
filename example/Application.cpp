#include "Application.hpp"

#include <unicorn/system/CustomValue.hpp>
#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/geometry/Primitives.hpp>

#include <functional>

Application::Application(unicorn::Settings& settings, unicorn::UnicornRender* pRender)
    : m_pRender(pRender)
    , m_timer(true)
    , m_pCameraController(nullptr)
    , m_pVkRenderer(nullptr)
    , m_physicsWorld(m_particles
        , m_physicsForceRegistry
        , m_physicsContactGenerators
        , MAX_OBJECT_COUNT * MAX_OBJECT_COUNT
        , 5000)
{
    unicorn::video::Graphics* pGraphics = m_pRender->GetGraphics();

    pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
                                     unicorn::system::CustomValue::True);

    pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
                                     unicorn::system::CustomValue::True);

    auto h = pGraphics->GetMonitors().back()->GetActiveVideoMode().height;
    auto w = pGraphics->GetMonitors().back()->GetActiveVideoMode().width;

    settings.SetApplicationHeight(h);
    settings.SetApplicationWidth(w);

    unicorn::system::Window* pWindow = pGraphics->SpawnWindow(settings.GetApplicationWidth(),
        settings.GetApplicationHeight(),
        settings.GetApplicationName(),
        nullptr,
        nullptr);

    pWindow->SetMouseMode(unicorn::system::MouseMode::Captured);

    m_pVkRenderer = pGraphics->SpawnRenderer(pWindow);

    m_pVkRenderer->SetBackgroundColor(unicorn::video::Color::LightPink);
    m_pCameraController = new unicorn::video::CameraFpsController(m_pVkRenderer->GetCamera());

    // Binds
    {
        using namespace std::placeholders;

        m_pRender->LogicFrame.connect(this, &Application::OnLogicFrame);

        m_pVkRenderer->Destroyed.connect(this, &Application::OnRendererDestroyed);

        pWindow->MouseButton.connect(this, &Application::OnMouseButton);
        pWindow->MousePosition.connect(this, &Application::OnCursorPositionChanged);
        pWindow->Scroll.connect(this, &Application::OnMouseScrolled);
        pWindow->Keyboard.connect(this, &Application::OnWindowKeyboard);
    }

    SceneReset();
}

Application::~Application()
{
    delete m_pCameraController;

    if (m_pVkRenderer)
    {
        for (auto& pMeshDescriptor : m_worldObjects)
        {
            m_pVkRenderer->DeleteMesh(&pMeshDescriptor->GetMesh());
            delete pMeshDescriptor;
        }

        delete m_pVkRenderer;
    }
}

void Application::Run()
{
    m_pRender->Run();
}

void Application::OnLogicFrame(unicorn::UnicornRender* /*render*/)
{
    using unicorn::video::geometry::MeshDescriptor;

    float currentFrame = static_cast<float>(m_timer.ElapsedMilliseconds().count()) / 1000;
    float newDeltatime = currentFrame - m_lastFrame;

    if (newDeltatime <= 0.0)
    {
        return;
    }

    m_deltaTime = newDeltatime;

    m_physicsWorld.StartFrame();

    m_physicsWorld.RunPhysics(0.01);

    MeshDescriptor* pMeshDescriptor = nullptr;

    for (auto const& body : m_rigidBodies)
    {
        pegasus::Particle& particle = body.p;
        body.s->SetCenterOfMass(particle.GetPosition());
        pMeshDescriptor = m_glue[&particle];

        if (pMeshDescriptor != m_worldObjects.front())
        {
            pMeshDescriptor->SetIdentity();
            pMeshDescriptor->Translate(particle.GetPosition());
        }
    }

    m_lastFrame = currentFrame;
}

void Application::OnMouseButton(unicorn::system::Window::MouseButtonEvent const& mouseButtonEvent)
{
    using unicorn::system::input::MouseButton;
    using unicorn::system::input::Action;
    using unicorn::video::geometry::MeshDescriptor;
    using unicorn::video::geometry::Primitives;

    unicorn::system::input::Action const& action = mouseButtonEvent.action;

    if (action == Action::Release || action == Action::Repeat)
    {
        return;
    }

    unicorn::system::input::MouseButton const& button = mouseButtonEvent.button;

    switch (button)
    {
        case MouseButton::MouseLeft:
        {
            SpawnCube();

            break;
        }
        case MouseButton::MouseRight:
        {
            DeleteCube();

            break;
        }
        default:
        {
            break;
        }
    }
}

void Application::OnCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    m_pCameraController->UpdateView(pos.first, pos.second);
}

void Application::OnMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    m_pCameraController->Scroll(static_cast<float>(pos.second / 50)); // 50 is zoom coefficient
}

void Application::OnWindowKeyboard(unicorn::system::Window::KeyboardEvent const& keyboardEvent)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;
    using unicorn::system::MouseMode;

    unicorn::system::input::Action const& action = keyboardEvent.action;

    if (Action::Release == action)
    {
        return;
    }

    float delta = m_deltaTime * 0.1f;

    {
        unicorn::system::input::Modifier::Mask const& modifiers = keyboardEvent.modifiers;

        if (Modifier::Shift & modifiers)
        {
            delta *= 10;
        }

        if (Modifier::Alt & modifiers)
        {
            delta *= 5;
        }
    }

    switch (keyboardEvent.key)
    {
        case Key::W:
        {
            m_pCameraController->MoveForward(delta);
            break;
        }
        case Key::S:
        {
            m_pCameraController->MoveBackward(delta);
            break;
        }
        case Key::A:
        {
            m_pCameraController->MoveLeft(delta);
            break;
        }
        case Key::D:
        {
            m_pCameraController->MoveRight(delta);
            break;
        }
        case Key::Q:
        {
            m_pCameraController->MoveUp(delta);
            break;
        }
        case Key::E:
        {
            m_pCameraController->MoveDown(delta);
            break;
        }
        case Key::C:
        {
            keyboardEvent.pWindow->SetMouseMode(MouseMode::Captured);
            break;
        }
        case Key::J:
        {
            SceneReset();
            break;
        }
        case Key::Escape:
        {
            keyboardEvent.pWindow->SetMouseMode(MouseMode::Normal);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Application::OnRendererDestroyed(unicorn::video::Renderer* pRenderer)
{
    if (m_pVkRenderer == pRenderer)
    {
        m_pVkRenderer = nullptr;
    }
}

void Application::SpawnCube()
{
    using unicorn::video::geometry::MeshDescriptor;
    using unicorn::video::geometry::Primitives;

    glm::dvec3 position = {0.0, 0.0, 20.0};

    // Render part
    MeshDescriptor* pCube = new MeshDescriptor(Primitives::Cube(*(m_pVkRenderer->SpawnMesh())));
    pCube->Translate(position);
    pCube->SetColor({static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255});
    m_cubes.push_back(pCube);

    // Physics part
    static auto randDouble = []()
    {
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(-5.0, 5.0);
        return distribution(generator);
    };

    m_particles.emplace_back();

    pegasus::Particle& particle = m_particles.back();
    particle.SetPosition(position);
    particle.SetDamping(1.0f);
    particle.SetVelocity(randDouble(), randDouble() - 5, randDouble());

    m_rigidBodies.emplace_back(
        particle,
        std::make_unique<pegasus::geometry::Box>(
            position,
            glm::dvec3{1.0f, 0, 0},
            glm::dvec3{0, 1.0f, 0},
            glm::dvec3{0, 0, 1.0f}
        )
    );

    m_physicsForceRegistry.Add(particle, *m_forces.front());

    m_physicsContactGenerators.push_back(
        std::make_unique<pegasus::ShapeContactGenerator<RigidBodies>>(m_rigidBodies.back()
            , m_rigidBodies
            , (randDouble() + 5) / 10)
    );

    // Glue
    m_glue[&particle] = pCube;
}

void Application::DeleteCube()
{
    if (m_cubes.size())
    {
        // Get random cube
        const uint32_t index = std::rand() % m_cubes.size();

        // Render part
        {
            auto meshIt = m_cubes.begin();

            std::advance(meshIt, index);

            // Fetch cube's mesh
            auto const& mesh = (*meshIt)->GetMesh();

            // Erase cube
            m_cubes.erase(meshIt);

            // Release cube's mesh
            m_pVkRenderer->DeleteMesh(&mesh);
        }

        // Physics part
        {
            auto particleIt = m_particles.begin();
            auto rigidBodyIt = m_rigidBodies.begin();
            auto physicsCGIt = m_physicsContactGenerators.begin();

            // There is offset in particles and rigid bodies for world objects
            std::advance(particleIt, index + m_worldObjects.size());
            std::advance(rigidBodyIt, index + m_worldObjects.size());
            std::advance(physicsCGIt, index);

            m_physicsForceRegistry.Remove(*particleIt);

            m_physicsContactGenerators.erase(physicsCGIt);
            m_rigidBodies.erase(rigidBodyIt);
            m_particles.erase(particleIt);
        }
    }
}

void Application::SceneReset()
{
    // Step 1. Clear
    // Glue
    m_glue.clear();

    // Physics part
    m_rigidBodies.clear();
    m_forces.clear();
    m_particles.clear();
    m_physicsContactGenerators.clear();
    m_physicsForceRegistry.Clear();

    // Render part
    if (m_pVkRenderer)
    {
        for (auto& pMeshDescriptor : m_cubes)
        {
            m_pVkRenderer->DeleteMesh(&pMeshDescriptor->GetMesh());
            delete pMeshDescriptor;
        }

        for (auto& pMeshDescriptor : m_worldObjects)
        {
            m_pVkRenderer->DeleteMesh(&pMeshDescriptor->GetMesh());
            delete pMeshDescriptor;
        }
    }

    m_worldObjects.clear();
    m_cubes.clear();

    // Step 2. Setup
    // Create forces
    m_forces.push_back(std::make_unique<pegasus::ParticleGravity>(glm::dvec3{0, 9.8, 0}));

    // Render part
    using unicorn::video::geometry::MeshDescriptor;
    using unicorn::video::geometry::Primitives;

    const glm::dvec3 renderPlanePosition = {0.0f, 0.0f, -20.0f};
    const glm::dvec3 physicsPlanePosition = {0.0f, 20.0f, 0.0f}; // REEEEEEEE

    MeshDescriptor* pPlane = new MeshDescriptor(Primitives::Quad(*(m_pVkRenderer->SpawnMesh())));
    pPlane->Rotate(glm::radians(90.0f), {1, 0, 0});
    pPlane->Translate(renderPlanePosition);
    pPlane->Scale({100.00f, 100.0f, 0.0f});
    pPlane->SetColor({0.18f, 0.31f, 0.31f});
    m_worldObjects.push_back(pPlane);

    // Physics part
    m_particles.emplace_back();
    pegasus::Particle& particle = m_particles.back();
    particle.SetPosition(physicsPlanePosition);
    particle.SetInverseMass(0);

    m_rigidBodies.emplace_back(
        particle,
        std::make_unique<pegasus::geometry::Plane>(
            particle.GetPosition(), glm::normalize(glm::dvec3{0.0, -1.0, 0.0})
        )
    );

    // Glue
    m_glue[&particle] = pPlane;
}
