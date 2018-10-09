/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_RENDER_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_RENDER_COMPONENT_HPP

#include <sleipnir/utility/Types.hpp>

#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/Material.hpp>

namespace sleipnir
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

}
}

#endif // SLEIPNIR_COMPONENT_RENDER_COMPONENT_HPP
