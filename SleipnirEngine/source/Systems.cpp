/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/Systems.hpp>

namespace sleipnir
{

Systems::Systems()
{

}

void Systems::Add(system::ISystem* pSystem, uint16_t priority)
{
    assert(nullptr != pSystem);

    m_systems.emplace(pSystem, priority);
}

void System::Delete(system::ISystem* pSystem)
{
    std::multiset<Entry>::const_iterator systemsIt = m_systems.begin();

    while (systemsIt != m_systems.end())
    {
        if (*systemsIt != pSystem)
        {
            ++systemsIt;
        }
        else
        {
            // per documentation std::multiset::erase does not invalidate other
            // pointers or references
            systemsIt = m_systems.erase(systemsIt);
        }
    }
}

}
