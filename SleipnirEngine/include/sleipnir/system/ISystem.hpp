/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_ISYSTEM_HPP
#define SLEIPNIR_SYSTEM_ISYSTEM_HPP

namespace sleipnir
{
namespace system
{

//! System interface
class ISystem
{
public:
    //! Default constructor
    ISystem() = default;

    //! Default destructor
    virtual ~ISystem() = default;

    //! Method invoked each loop cycle
    virtual void Update() = 0;
};

}
}

#endif // SLEIPNIR_SYSTEM_ISYSTEM_HPP
