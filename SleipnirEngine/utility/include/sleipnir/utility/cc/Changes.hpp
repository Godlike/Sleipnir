/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_CC_CHANGES_HPP
#define SLEIPNIR_UTILITY_CC_CHANGES_HPP

#include <sleipnir/ecs/WorldTime.hpp>

#include <mutex>
#include <queue>
#include <utility>
#include <vector>

namespace sleipnir
{
namespace utility
{
namespace cc
{

template<typename TMemento, typename TTimeUnit = ecs::WorldTime::TimeUnit>
class Changes
{
public:
    using Snapshot = TMemento;
    using Object = typename Snapshot::Object;
    using ModifyOperation = Object& (Object::*)(Snapshot const&);

    using AddCollection = std::vector<Snapshot>;
    using ModifyCollection = std::vector< std::pair<Snapshot, ModifyOperation> >;
    using DeleteCollection = std::vector<Snapshot>;

    class Instance
    {
    public:
        Instance(Instance const& other) = default;
        Instance& operator=(Instance const& other) = default;

        Instance(Instance&& other) = default;
        Instance& operator=(Instance&& other) = default;

        bool operator<(Instance const& other) const;

        bool IsEmpty() const;
        uint16_t GetPriority() const { return m_priority; }

        void Add(Snapshot entry);
        void Modify(Snapshot entry, ModifyOperation operation);
        void Delete(Snapshot entry);

        void Reset();

        void Push(TTimeUnit timestamp);

        void Export(AddCollection& adds, ModifyCollection& modifies, DeleteCollection& deletes);

    private:
        friend class Changes;

        Instance(Changes* pParent, uint16_t priority = 0x8000);

        void Merge(Instance const& other);

        Changes* m_pParent;
        uint16_t m_priority;

        AddCollection m_add;
        ModifyCollection m_modify;
        DeleteCollection m_delete;
    };

    template<class TCollection>
    class Integrator
    {
    public:
        Integrator(TCollection& collection);

        void Integrate(Changes::Instance& diff);

    private:
        TCollection& m_collection;
    };

    Changes() = default;

    Changes(Changes const& other) = delete;
    Changes& operator=(Changes const& other) = delete;

    Changes(Changes&& other) = delete;
    Changes& operator=(Changes&& other) = delete;

    ~Changes() = default;

    Instance Clone(uint16_t priority = 0x8000);

    void Push(Instance& instance, TTimeUnit timestamp);

    Instance Pull(TTimeUnit timestamp);

private:
    std::mutex m_hostMutex;

    struct PushCommand
    {
        PushCommand(Instance& instance, TTimeUnit timestamp);
        bool operator<(PushCommand const& other) const;

        Instance instance;
        TTimeUnit timestamp;
    };

    std::priority_queue<PushCommand> m_pushes;

};

}
}
}

#include <sleipnir/utility/cc/Changes.imp>

#endif // SLEIPNIR_UTILITY_CC_CHANGES_HPP
