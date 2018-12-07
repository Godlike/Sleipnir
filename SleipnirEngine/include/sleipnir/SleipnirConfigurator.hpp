#ifndef SLEIPNIR_SLEIPNIR_CONFIGURATOR_HPP
#define SLEIPNIR_SLEIPNIR_CONFIGURATOR_HPP

#include <tulpar/Loggers.hpp>
#include <tulpar/TulparConfigurator.hpp>

#include <unicorn/Loggers.hpp>
#include <unicorn/utility/Settings.hpp>

// #include <pegasus/Loggers.hpp>

#include <sleipnir/ecs/system/Lifetime.hpp>
#include <sleipnir/Loggers.hpp>

namespace sleipnir
{

class SleipnirConfigurator
{
public:
    mule::LoggerConfigBase::Settings globalLoggerSettings;

    struct Tulpar
    {
        tulpar::Loggers::Settings* loggerOverride = nullptr;
        tulpar::TulparConfigurator* config = nullptr;
    } tulpar;

    struct Unicorn
    {
        unicorn::Loggers::Settings* loggerOverride = nullptr;
        unicorn::utility::Settings* config = nullptr;
    } unicorn;

    struct Pegasus
    {
        // pegasus::Loggers::Settings* loggerOverride = nullptr;
        void* config = nullptr;
    } pegasus;

    struct Sleipnir
    {
        sleipnir::Loggers::Settings* loggerOverride = nullptr;
    } sleipnir;

    struct Lifetime
    {
        ecs::system::Lifetime::EntityReclaimer reclaimer = nullptr;
    } lifetime;
};

}

#endif // SLEIPNIR_SLEIPNIR_CONFIGURATOR_HPP
