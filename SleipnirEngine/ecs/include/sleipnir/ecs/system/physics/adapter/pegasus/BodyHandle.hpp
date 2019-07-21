/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HANDLE_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HANDLE_HPP

#include <pegasus/Asset.hpp>

#include <atomic>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{
namespace adapter
{
namespace pegasus
{

//! Holds pegasus body handle
struct BodyHandle
{
    //! Atomic pegasus body handle
    std::atomic<::pegasus::scene::Handle> bodyHandle;
};

}
}
}
}
}
}

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HANDLE_HPP
