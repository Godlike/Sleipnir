/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_POSITION_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_POSITION_COMPONENT_HPP

#include <sleipnir/utility/Types.hpp>

#include <glm/glm.hpp>

namespace sleipnir
{
namespace component
{

//! Position description
struct PositionComponent : public Component
{
    //! Position
    glm::vec3 position;
};

}
}

#endif // SLEIPNIR_COMPONENT_POSITION_COMPONENT_HPP
