/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_SYSTEM_PHYSICS_BODY_CONTROLLER_HPP
#define SLEIPNIR_SYSTEM_PHYSICS_BODY_CONTROLLER_HPP

#include <sleipnir/WorldTime.hpp>

#include <sleipnir/system/physics/BodyHandle.hpp>
#include <sleipnir/system/physics/PegasusAdapter.hpp>
#include <sleipnir/system/physics/SpawnInfo.hpp>

#include <glm/glm.hpp>

#include <list>
#include <mutex>
#include <set>

namespace sleipnir
{
namespace system
{
namespace physics
{

/** @brief  Performs operations on physics bodies
 *
 *  Queues all requests and executes them when time comes
 */
class BodyController
{
public:
    /** @brief  Basic constructor
     *
     *  @param  physicsEngine   physics engine adapter
     */
    BodyController(PegasusAdapter& physicsEngine);

    BodyController(BodyController const& other) = delete;
    BodyController& operator=(BodyController const& other) = delete;

    //! Default destructor
    ~BodyController() = default;

    //! Helper structure handling spawn orders
    struct Spawner
    {
        //! Helper structure describing spawn order
        struct Order
        {
            //! Pointer to body handle
            BodyHandle* pHandle;

            //! Time when order shall be executed
            WorldTime::TimeUnit spawnTime;

            //! Body information
            SpawnInfo info;
        };

        //! Mutex protecting @ref orders
        std::mutex mutex;

        //! Collection of orders
        std::list<Order> orders;
    };

    //! Helper structure handling push orders
    struct Pusher
    {
        //! Helper structure describing push order
        struct Order
        {
            //! Pointer to body handle
            BodyHandle const* pHandle;

            //! Time when order shall be executed
            WorldTime::TimeUnit pushTime;

            //! Force information
            glm::vec3 force;
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
            //! Pointer to body handle
            BodyHandle const* pHandle;

            //! Time when order shall be executed
            WorldTime::TimeUnit deleteTime;
        };

        //! Mutex protecting @ref orders
        std::mutex mutex;

        //! Collection of orders
        std::list<Order> orders;
    };

    //! Enqueue spawn @p order to be executed
    void Spawn(Spawner::Order order);

    //! Enqueue push @p order to be executed
    void Push(Pusher::Order order);

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

    //! Execute push orders based on @p currentTime
    void CheckPusher(WorldTime::TimeUnit currentTime);

    //! Execute spawn orders based on @p currentTime
    void CheckSpawner(WorldTime::TimeUnit currentTime);

    //! Physics engine adapter
    PegasusAdapter& m_physicsEngine;

    //! Spawn order holder
    Spawner m_spawner;

    //! Push order holder
    Pusher m_pusher;

    //! Delete order holder
    Deleter m_deleter;

    //! Set of deleted handles during Check() execution
    std::set<BodyHandle const*> m_deletedHandles;
};

}
}
}

#endif // SLEIPNIR_SYSTEM_PHYSICS_BODY_CONTROLLER_HPP
