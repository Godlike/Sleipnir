/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTIL_TEMPLATE_COUNTER_HPP
#define SLEIPNIR_UTIL_TEMPLATE_COUNTER_HPP

#include <cstddef>

namespace sleipnir
{
namespace util
{

/** @brief  Static template counter
 *
 *  Allows generating ids for arbitrary typenames
 *
 *  @tparam CounterName name of counter instance
 */
template<typename CounterName>
    class TemplateCounter
{
public:
    /** @brief  Return unique id corresponding to @p T
     *
     *  @note   uniqueness is guaranteed only within the same @p CounterName
     *
     *  @tparam T   arbitrary typename
     *
     *  @return id of @p T
     */
    template<typename T>
        static std::size_t GetId()
        {
            static const std::size_t id = m_counter++;
            return id;
        }

private:
    //! Number of generated ids
    static std::size_t m_counter;
};

template<typename CounterName>
    std::size_t TemplateCounter<CounterName>::m_counter = 0;

}
}

#endif // SLEIPNIR_UTIL_TEMPLATE_COUNTER_HPP
