/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SLEIPNIR_ENGINE_HPP
#define SLEIPNIR_SLEIPNIR_ENGINE_HPP

#include <sleipnir/Systems.hpp>

namespace sleipnir
{

/** @brief  Sleipnir library entry point */
class SleipnirEngine
{
public:
    /** @brief  Creates library instance
     *
     */
    SleipnirEngine();

    //! Disable copy constructor
    SleipnirEngine(SleipnirEngine const& other) = delete;

    //! Disable assignment operator
    SleipnirEngine& operator=(SleipnirEngine const& other) = delete;

    //! Deinitialize library instance
    ~SleipnirEngine();

    /** @brief  Initialize library instance
     *
     *  Using provided @p config initializes required subsystems
     *
     *  @param  config  sleipnir configuration
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    bool Initialize(SleipnirConfigurator const& config);

    /** @brief  Reinitializes library instance
     *
     *  Using provided @p config reinitializes all required subsystems and
     *  deinitializes all previously initialized subsystems that are missing
     *  from @p config
     *
     *  @param  config  sleipnir configuration
     *
     *  @return @c true if reinitialization was successful, @c false otherwise
     */
    bool Reinitialize(SleipnirConfigurator const& config);

    /** @brief  Deinitializes all previously initialized subsystems */
    void Deinitialize();

    /** @brief  Returns a reference to ECS system controller */
    Systems& GetSystems() { return m_systems; }

private:
    //! ECS system collection
    Systems m_systems;
};

}

#endif // SLEIPNIR_SLEIPNIR_ENGINE_HPP
