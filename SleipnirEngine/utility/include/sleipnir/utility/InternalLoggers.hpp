/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_INTERNAL_LOGGERS_HPP
#define SLEIPNIR_UTILITY_INTERNAL_LOGGERS_HPP

#include <mule/Logger.hpp>

#include <array>
#include <cstdint>

namespace sleipnir
{
namespace utility
{

struct Log
{
    static constexpr uint32_t sleipnir = 0;
    static constexpr uint32_t sleipnir_profile = 1;

    static constexpr uint32_t size = 2;
};

extern std::array<mule::LoggerPtr, Log::size> g_loggers;

} // namespace utility
} // namespace sleipnir

#define LOG ::sleipnir::utility::g_loggers[::sleipnir::utility::Log::sleipnir]
#define LOG_PROFILE ::sleipnir::utility::g_loggers[::sleipnir::utility::Log::sleipnir_profile]

#endif // SLEIPNIR_UTILITY_INTERNAL_LOGGERS_HPP
