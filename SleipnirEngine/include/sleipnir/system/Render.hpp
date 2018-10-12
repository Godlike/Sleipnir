/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_RENDER_HPP
#define SLEIPNIR_SYSTEM_RENDER_HPP

#include <sleipnir/entity/World.hpp>

#include <sleipnir/component/Flags.hpp>
#include <sleipnir/component/PositionComponent.hpp>
#include <sleipnir/component/RenderComponent.hpp>

#include <sleipnir/system/Skeleton.hpp>

#include <unicorn/utility/Settings.hpp>
#include <unicorn/UnicornRender.hpp>

#include <unicorn/video/Camera.hpp>
#include <unicorn/video/CameraFpsController.hpp>
#include <unicorn/video/Material.hpp>
#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/PerspectiveCamera.hpp>

namespace sleipnir
{
namespace system
{

/** @brief  System responsible for rendering objects */
class Render : public Skeleton<component::PositionComponent, component::RenderComponent>
{
public:
    //! Shortcut to renderable mesh
    using Mesh = unicorn::video::Mesh;

    //! Shortcut to render material
    using Material = unicorn::video::Material;

    /** @brief  Basic constructor
     *
     *  @param  world   entity world
     */
    Render(entity::World& world);

    /** @brief  Basic destructor
     *
     *  Deinitializes rendering subsystem
     */
    ~Render();

    /** @brief  Initializes rendering subsystem
     *
     *  @param  settings    unicorn settings
     *  @param  render      rendering subsystem
     */
    void Initialize(unicorn::utility::Settings& settings, unicorn::UnicornRender& render);

    /** @brief  Method invoked each loop cycle
     *
     *  Updates transformation matrix for all entities
     */
    void Update() override;

    /** @brief  Spawns renderable mesh object
     *
     *  @return newly created mesh
     *
     *  @sa DeleteMesh()
     */
    Mesh* SpawnMesh();

    /** @brief  Adds mesh to rendering subsystem
     *
     *  @todo   move this to SpawnMesh() once Unicorn#120 is resolved
     *
     *  @param  pMesh   pointer to renderable mesh
     *
     *  @sa SpawnMesh()
     */
    void AddMesh(Mesh* pMesh);

    /** @brief  Removes mesh from rendering subsystem
     *
     *  @param  pMesh   pointer to renderable mesh
     *
     *  @sa SpawnMesh()
     */
    void DeleteMesh(Mesh* pMesh);

    //! First person camera controller
    unicorn::video::CameraFpsController* pCameraController;

private:
    /** @brief  Slot invoked when video renderer is destroyed
     *
     *  @param  pRenderer   destroyed renderer
     */
    void OnRendererDestroyed(unicorn::video::Renderer* pRenderer);

    //! Video renderer
    unicorn::video::Renderer* m_pVkRenderer;

    //! Camera data object
    unicorn::video::Camera m_camera;

    //! Perspective camera projection controller
    unicorn::video::PerspectiveCamera* pCameraProjection;
};

}
}

#endif // SLEIPNIR_SYSTEM_RENDER_HPP
