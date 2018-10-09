/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTIL_TYPES_HPP
#define SLEIPNIR_UTIL_TYPES_HPP

#include <sleipnir/utility/TemplateCounter.hpp>

#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

namespace sleipnir
{

typedef uint32_t EntityId;
static EntityId const InvalidEntityId = std::numeric_limits<EntityId>::max();

//! Placeholder class for basic RTTI implementation
struct Component
{
    /** @brief  Returns unique id corresponding to @p C
     *
     *  @tparam C   component class derived from Component
     *
     *  @return id of @p C
     */
    template<class C>
        static inline std::size_t GetId()
    {
        static_assert(true == std::is_base_of<Component, C>::value, "C shall be derived from Component");

        return utility::TemplateCounter<Component>::GetId<C>();
    }
};

//! Shortcut to unique_ptr of Component
typedef std::unique_ptr<Component> ComponentPtr;

}

#endif // SLEIPNIR_UTIL_TYPES_HPP
