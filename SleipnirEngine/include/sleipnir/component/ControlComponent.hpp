/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_CONTROL_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_CONTROL_COMPONENT_HPP

#include <sleipnir/util/Types.hpp>

#include <glm/glm.hpp>

namespace sleipnir
{
namespace component
{

//! Physics control description
struct ControlComponent : public Component
{
    //! Force to be applied to an object
    glm::vec3 force;
};

}
}

#endif // SLEIPNIR_COMPONENT_CONTROL_COMPONENT_HPP
