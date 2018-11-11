/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_RENDER_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_RENDER_COMPONENT_HPP

#include <sleipnir/ecs/Types.hpp>

#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/Material.hpp>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Render description
struct RenderComponent : public Component
{
    //! Pointer to renderable mesh
    unicorn::video::Mesh* pMesh;

    //! Rotation angle
    float rotateAngle = 0.0f;

    //! Rotation axes
    glm::vec3 rotateAxes;
};

} // namespace component
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_COMPONENT_RENDER_COMPONENT_HPP
