/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HPP
#define SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HPP

#include <sleipnir/ecs/system/physics/BodyHandle.hpp>

#include <pegasus/Asset.hpp>
#include <pegasus/Primitives.hpp>
#include <pegasus/Scene.hpp>

#include <memory>
#include <utility>
#include <unordered_map>

namespace sleipnir
{
namespace ecs
{
namespace system
{
namespace physics
{
namespace adapter
{
namespace pegasus
{

class BodyObject;
class BodyCollection;

/** @brief  Physics body data snapshot object
 *
 *  Represents a snapshot of data to be applied to physics body object
 *
 *  Variables that can be applied to physics body object are stored as a pair
 *  of <valid, data> values:
 *  - @p valid is a flag if this data is set
 *  - @p data is value to be used
 */
struct BodyMemento
{
    //! Shortcut to associated object type
    using Object = BodyObject;

    //! Shortcut to underlying handle type
    using Handle = ::pegasus::scene::Handle;

    //! ID to be used when body handle is unknown/unavailable
    constexpr static Handle UNKNOWN_ID = ::pegasus::scene::ZERO_HANDLE;

    //! Linear motion data wrapper
    struct LinearMotion
    {
        //! Body position in space
        glm::vec3 position = glm::vec3{0};

        //! Body linear velocity
        glm::vec3 velocity = glm::vec3{0};

        //! Body linear acceleration
        glm::vec3 acceleration = glm::vec3{0};

        //! Body linear force
        glm::vec3 force = glm::vec3{0};
    };

    //! Angular motion data wrapper
    struct AngularMotion
    {
        //! Body orientation in space
        glm::quat orientation = glm::vec3{0};

        //! Body angular velocity
        glm::vec3 velocity = glm::vec3{0};

        //! Body angular acceleration
        glm::vec3 acceleration = glm::vec3{0};

        //! Body angular torque
        glm::vec3 torque = glm::vec3{0};
    };

    /** @brief  Pegasus body handle
     *
     *  Used to lookup physics object
     */
    Handle handle = UNKNOWN_ID;

    //! Pointer to arion shape
    arion::SimpleShape* pShape = nullptr;

    /** @brief  Body linear motion
     *
     *  A pair of <valid, data> values:
     *  - @p valid is a flag if this data is set
     *  - @p data is LinearMotion values to be used
     *
     *  If @p valid is set to @c true, the @p data is accessed and applied to
     *  physics body object
     */
    std::pair<bool, LinearMotion> linear = {false, {}};

    /** @brief  Body angular motion
     *
     *  A pair of <valid, data> values:
     *  - @p valid is a flag if this data is set
     *  - @p data is AngularMotion values to be used
     *
     *  If @p valid is set to @c true, the @p data is accessed and applied to
     *  physics body object
     */
    std::pair<bool, AngularMotion> angular = {false, {}};

    /** @brief  Body mass
     *
     *  A pair of <valid, data> values:
     *  - @p valid is a flag if this data is set
     *  - @p data is mass to be used
     *
     *  If @p valid is set to @c true, the @p data is accessed and applied to
     *  physics body object
     */
    std::pair<bool, double> mass = {false, 0.0};

    /** @brief  Damping
     *
     *  A pair of <valid, data> values:
     *  - @p valid is a flag if this data is set
     *  - @p data is damping to be used
     *
     *  If @p valid is set to @c true, the @p data is accessed and applied to
     *  physics body object
     */
    std::pair<bool, double> damping = {false, 0.0};
};

/** @brief  Physics body object
 *
 *  Represents state of physics body object
 */
class BodyObject
{
public:
    //! Shortcut to associated data snapshot type
    using Memento = BodyMemento;

    //! Shortcut to underlying handle type
    using Handle = ::pegasus::scene::Handle;

    //! ID to be used when body handle is unknown/unavailable
    constexpr static Handle UNKNOWN_ID = Memento::UNKNOWN_ID;

    /** @brief  Deleted copy constructor */
    BodyObject(BodyObject const& other) = delete;

    /** @brief  Deleted assignment operator */
    BodyObject& operator=(BodyObject const& other) = delete;

    /** @brief  Deleted move constructor */
    BodyObject(BodyObject&& other) = delete;

    /** @brief  Deleted move-assignment operator */
    BodyObject& operator=(BodyObject&& other) = delete;

    /** @brief  Updates physics data with given @p memento
     *
     *  @param  memento new object data
     *
     *  @note   Processes the following @p memento members:
     *          - linear
     *          - angular
     *          - mass
     *          - memento
     *
     *  @return reference to @p this object
     */
    BodyObject& operator=(Memento const& memento);

    /** @brief  Increments physics data with given @p memento
     *
     *  @param  memento new object data
     *
     *  @note   Processes the following @p memento members:
     *          - linear
     *          - angular
     *          - mass
     *          - memento
     *
     *  @return reference to @p this object
     */
    BodyObject& operator+=(Memento const& memento);

    /** @brief  Multiplies physics data with given @p memento
     *
     *  @param  memento new object data
     *
     *  @note   Processes the following @p memento members:
     *          - linear
     *          - angular
     *          - mass
     *          - memento
     *
     *  @return reference to @p this object
     */
    BodyObject& operator*=(Memento const& memento);

    glm::vec3 GetPosition() const;

private:
    friend class BodyCollection;

    /** @brief  Object constructor
     *
     *  @param  memento         object data
     *  @param  scene           reference to physics scene
     *  @param  primitivePtr    reference to shared_ptr controlling the primitive lifetime
     *
     *  @note   memento.pShape is used to set physics primitive
     *          calls operator= to set body properties
     */
    BodyObject(Memento const& memento
        , BodyCollection* pParent
        , ::pegasus::scene::Scene& scene
    );

    /** @brief  Default object destructor */
    ~BodyObject();

    //! Physics body handle
    Handle m_handle;

    //! Reference to physics world
    ::pegasus::scene::Scene& m_scene;

    //! Primitive associated with this body
    ::pegasus::scene::Primitive* m_pPrimitive;
};

/** @brief  Physics body controller
 *
 *  Spawns, holds and deletes physics body objects
 */
class BodyCollection
{
public:
    //! Shortcut to body object
    using Object = BodyObject;

    //! Shortcut to a collection of ::pegasus::scene::Primitive pointers
    using Primitives = std::list<::pegasus::scene::Primitive*>;

    //! Shortcut to a collection of created objects
    using Collection = std::unordered_map<BodyObject::Handle, Object*>;

    /** @brief  Object constructor
     *
     *  @param  scene   reference to physics scene
     */
    BodyCollection(::pegasus::scene::Scene& scene);

    /** @brief  Deleted copy constructor */
    BodyCollection(BodyCollection const& other) = delete;

    /** @brief  Deleted assignment operator */
    BodyCollection& operator=(BodyCollection const& other) = delete;

    /** @brief  Deleted move constructor */
    BodyCollection(BodyCollection&& other) = delete;

    /** @brief  Deleted move-assignment operator */
    BodyCollection& operator=(BodyCollection&& other) = delete;

    /** @brief  Object destructor
     *
     *  Deletes all spawned physics body objects
     */
    ~BodyCollection();

    /** @brief  Spawn physics body object
     *
     *  @param  pHandle     pointer to BodyHandle containing body handle for lookup
     *  @param  mememnto    initial body data snapshot
     *
     *  @return pointer to a spawned physics body object
     */
    Object* Spawn(BodyHandle* pHandle, Object::Memento const& memento);

    /** @brief  Get physics body object
     *
     *  @param  pHandle pointer to BodyHandle containing body handle for lookup
     *
     *  @return pointer to physics body object if found, @c nullptr otherwise
     */
    Object* Get(BodyHandle* pHandle) const;

    /** @brief  Deletes physics body object
     *
     *  @param  pHandle pointer to BodyHandle containing body handle for lookup
     *
     *  @return @c true if object was found and deleted, @c false otherwise
     */
    bool Delete(BodyHandle* pHandle);

    Primitives const& GetPrimitives() const { return m_primitives; }
    std::size_t GetPrimitiveCount() const { return m_primitiveCount; }

    Collection const& GetObjectCollection() const { return m_collection; }

private:
    //! Pegasus world
    ::pegasus::scene::Scene& m_scene;

    //! Collection of created objects
    Collection m_collection;

    //! Collection of primitives
    Primitives m_primitives;

    //! Number of primitives in @p primitives
    std::size_t m_primitiveCount;
};

}
}
}
}
}
}

#endif // SLEIPNIR_ECS_SYSTEM_PHYSICS_ADAPTER_PEGASUS_BODY_HPP
