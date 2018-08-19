/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ENTITY_ENTITY_HPP
#define SLEIPNIR_ENTITY_ENTITY_HPP

#include <sleipnir/util/Types.hpp>

namespace sleipnir
{
namespace entity
{

class World;

/** @brief  Entity object
 *
 *  Provides interface for component-related operations
 */
class Entity
{
public:
    //! Constructs invalid entity
    Entity();

    /** @brief  Basic constructor
     *
     *  @param  world   parent world
     *  @param  id      entity id
     */
    Entity(World* world, EntityId id);

    Entity(const Entity& other) = default;
    Entity& operator=(const Entity& other) = default;

    //! Default destructor
    ~Entity() = default;

    /** @brief  Compares two Entity objects
     *
     *  @param  other   entity
     *
     *  @return @c true if entities are equal, @c false otherwise
     *
     *  @sa operator!=()
     */
    bool operator==(const Entity& other) const;

    /** @brief  Compares two Entity objects
     *
     *  @param  other   entity
     *
     *  @return @c false if entities are equal, @c true otherwise
     *
     *  @sa operator==()
     */
    bool operator!=(const Entity& other) const { return !operator==(other); }

    //! Returns entity id
    EntityId GetId() const { return m_id; }

    //! Checks if entity-related operations can be performed
    bool IsValid() const;

    //! Destroys entity invalidating all instances of it
    void Destroy();

    /** @brief  Add component
     *
     *  @tparam C       component class
     *  @tparam Args    arguments required to construct component class
     *
     *  @param  args    arguments used to construct component class
     *
     *  @return reference to created component
     *
     *  @sa DeleteComponent()
     */
    template<class C, typename... Args>
        C& AddComponent(Args&&... args);

    /** @brief  Checks if entity has given component set
     *
     *  @tparam C   component class
     *
     *  @return @c true if entity has component, @c false otherwise
     *
     *  @sa GetComponent()
     */
    template<class C>
        bool HasComponent() const;

    /** @brief  Returns requested component
     *
     *  @tparam C   component class
     *
     *  @return reference to associated component
     *
     *  @sa HasComponent()
     */
    template<class C>
        C& GetComponent() const;

    /** @brief  Deletes requested component
     *
     *  @tparam C   component class
     *
     *  @sa AddComponent()
     */
    template<class C>
        void DeleteComponent();

private:
    /** @brief  Add component identified by @p componentId
     *
     *  @param  pComponent  pointer to component
     *  @param  componentId component id
     */
    void AddComponentImpl(Component* pComponent, std::size_t componentId);

    /** @brief  Get reference to component identified by @p componentId
     *
     *  @param  componentId component id
     *
     *  @return reference to base component
     */
    Component& GetComponentImpl(std::size_t componentId) const;

    /** @brief  Checked if entity has component identified by @p componentId
     *
     *  @param  componentId component id
     *
     *  @return @c true if entity has component, @c false otherwise
     */
    bool HasComponentImpl(std::size_t componentId) const;

    /** @brief  Deletes component identified by @p componentId
     *
     *  @param  componentId component id
     */
    void DeleteComponentImpl(std::size_t componentId);

    //! Entity id
    EntityId m_id;

    //! Parent world
    World* m_pWorld;
};

}
}

#include <sleipnir/entity/Entity.imp>

#endif // SLEIPNIR_ENTITY_ENTITY_HPP
