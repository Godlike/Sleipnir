/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_COMPONENT_PHYSICS_COMPONENT_HPP
#define SLEIPNIR_COMPONENT_PHYSICS_COMPONENT_HPP

#include <sleipnir/system/physics/PhysicsThread.hpp>

#include <sleipnir/utility/Types.hpp>

namespace sleipnir
{
namespace component
{

//! Physics description
struct PhysicsComponent : public Component
{
    //! Physics body handle
    system::physics::BodyHandle* pHandle;
};

}
}

#endif // SLEIPNIR_COMPONENT_PHYSICS_COMPONENT_HPP
