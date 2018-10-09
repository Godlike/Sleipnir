/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_CONFIG
#define SLEIPNIR_UTILITY_CONFIG

#include <cstddef>

#include <chrono>

namespace sleipnir
{
namespace utility
{
namespace Config
{

static const std::size_t MaxComponentCount = 32;
static const std::size_t MaxPhysicsObjects = 1024;
static const std::size_t InitialEntityCount = 1024;
static const std::chrono::microseconds PhysicsTick = std::chrono::microseconds(4000);

static const float GridSide = 5.0f;
static const float GridAltFactor = 1e-1f;

}
}
}

#endif // SLEIPNIR_UTILITY_CONFIG
