/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_INTERNAL_LOGGERS_HPP
#define SLEIPNIR_INTERNAL_LOGGERS_HPP

#include <mule/Logger.hpp>

#include <array>
#include <cstdint>

namespace sleipnir
{

struct Log
{
    static constexpr uint32_t sleipnir = 0;
    static constexpr uint32_t sleipnir_profile = 1;

    static constexpr uint32_t size = 2;
};

extern std::array<mule::LoggerPtr, Log::size> g_loggers;

}

#define LOG ::sleipnir::g_loggers[::sleipnir::Log::sleipnir]
#define LOG_PROFILE ::sleipnir::g_loggers[::sleipnir::Log::sleipnir_profile]

#endif // SLEIPNIR_INTERNAL_LOGGERS_HPP
