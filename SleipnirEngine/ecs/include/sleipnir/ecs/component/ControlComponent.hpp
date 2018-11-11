/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_CONTROL_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_CONTROL_COMPONENT_HPP

#include <sleipnir/ecs/Types.hpp>

#include <glm/glm.hpp>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Physics control description
struct ControlComponent : public Component
{
    //! Force to be applied to an object
    glm::vec3 force;
};

} // namespace component
} // namespace ecs
} // namespace sleipnir

#endif // SLEIPNIR_ECS_COMPONENT_CONTROL_COMPONENT_HPP
