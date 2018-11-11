/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_COMPONENT_POSITION_COMPONENT_HPP
#define SLEIPNIR_ECS_COMPONENT_POSITION_COMPONENT_HPP

#include <sleipnir/ecs/Types.hpp>

#include <glm/glm.hpp>

namespace sleipnir
{
namespace ecs
{
namespace component
{

//! Position description
struct PositionComponent : public Component
{
    //! Position
    glm::vec3 position;
};

} // namespace sleipnir
} // namespace ecs
} // namespace component

#endif // SLEIPNIR_ECS_COMPONENT_POSITION_COMPONENT_HPP
