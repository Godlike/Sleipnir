/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_BODY_HANDLE_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_BODY_HANDLE_HPP

#include <atomic>

#include <pegasus/Scene.hpp>

namespace sleipnir
{
namespace system
{
namespace physics
{

//! Holds pegasus body handle
struct BodyHandle
{
    //! Atomic pegasus body handle
    std::atomic<pegasus::scene::Handle> bodyHandle;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_BODY_HANDLE_HPP
