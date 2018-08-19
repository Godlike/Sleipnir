/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_DYNAMIC_FORCE_CONTROLLER_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_DYNAMIC_FORCE_CONTROLLER_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/system/physics/BodyHandle.hpp>
#include <sleipnir/system/physics/PegasusAdapter.hpp>
#include <sleipnir/system/physics/SpawnInfo.hpp>

#include <glm/glm.hpp>

#include <list>
#include <mutex>

namespace sleipnir
{
namespace system
{
namespace physics
{

/** @brief  Creates and deletes dynamic forces
 *
 *  Queues all requests and executes them when time comes
 */
class DynamicForceController
{
public:
    /** @brief  Basic constructor
     *
     *  @param  physicsEngine   physics engine adapter
     */
    DynamicForceController(PegasusAdapter& physicsEngine);

    DynamicForceController(DynamicForceController const& other) = delete;
    DynamicForceController& operator=(DynamicForceController const& other) = delete;

    //! Default destructor
    ~DynamicForceController() = default;

    //! Helper structure handling create orders
    struct Creator
    {
        //! Helper structure describing create order
        struct Order
        {
            //! Force id
            uint32_t id;

            //! Time when order shall be executed
            WorldTime::TimeUnit createTime;

            //! Force source position
            glm::vec3 position;

            //! Force magnitude
            double magnitude;
        };

        //! Mutex protecting @ref orders
        std::mutex mutex;

        //! Collection of orders
        std::list<Order> orders;
    };

    //! Helper structure handling delete orders
    struct Deleter
    {
        //! Helper structure describing delete order
        struct Order
        {
            //! Force id
            uint32_t id;

            //! Time when order shall be executed
            WorldTime::TimeUnit deleteTime;
        };

        //! Mutex protecting @ref orders
        std::mutex mutex;

        //! Collection of orders
        std::list<Order> orders;
    };

    //! Enqueue create @p order to be executed
    void Create(Creator::Order order);

    //! Enqueue delete @p order to be executed
    void Delete(Deleter::Order order);

    /** @brief  Execute queued orders based on @p currentTime
     *
     *  @param  currentTime time to compare orders against
     */
    void Check(WorldTime::TimeUnit currentTime);

private:
    //! Execute delete orders based on @p currentTime
    void CheckDeleter(WorldTime::TimeUnit currentTime);

    //! Execute create orders based on @p currentTime
    void CheckCreator(WorldTime::TimeUnit currentTime);

    //! Physics engine adapter
    PegasusAdapter& m_physicsEngine;

    //! Create order holder
    Creator m_creator;

    //! Delete order holder
    Deleter m_deleter;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_DYNAMIC_FORCE_CONTROLLER_HPP
