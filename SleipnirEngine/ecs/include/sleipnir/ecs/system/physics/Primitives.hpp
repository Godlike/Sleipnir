/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP

#include <sleipnir/ecs/system/physics/Body.hpp>
#include <sleipnir/ecs/system/physics/BodyHandle.hpp>
// #include <sleipnir/ecs/system/physics/Force.hpp>

#include <sleipnir/utility/cc/Changes.hpp>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{

using BodyChanges = sleipnir::utility::cc::Changes<BodyObject::Memento>;
using BodyIntegrator = BodyChanges::Integrator<BodyCollection>;

// using ForceChanges = sleipnir::utility::cc::Changes<ForceObject::Memento>;
// using ForceIntegrator = ForceChanges::Integrator<ForceCollection>;

}
}
}
}

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_PEGASUS_PRIMITIVES_HPP
