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
#include <tuple>
#include <unordered_map>
#include <vector>

namespace sleipnir
{
namespace utility
{
namespace cc
{

template<
    typename TMemento
    , typename THandle
    , typename TTimeUnit = ecs::WorldTime::TimeUnit
    , typename THandleCreator = THandle(*)()
    , typename THandleDeleter = void(*)(THandle)
>
class Changes
{
public:
    using Snapshot = TMemento;

    using Handle = THandle;
    using TimeUnit = TTimeUnit;
    using HandleCreator = THandleCreator;
    using HandleDeleter = THandleDeleter;

    using Object = typename Snapshot::Object;
    using ModifyOperation = Object& (Object::*)(Snapshot const&);

    using AddCollection = std::unordered_map<Handle, Snapshot>;
    using ModifyCollection = std::unordered_map<Handle, std::tuple<Snapshot, ModifyOperation> >;
    using DeleteCollection = std::vector<Handle>;

    class Instance
    {
    public:
        Instance(Instance const& other) = default;
        Instance& operator=(Instance const& other) = default;

        Instance(Instance&& other) = default;
        Instance& operator=(Instance&& other) = default;

        bool operator>(Instance const& other) const;

        bool IsEmpty() const;
        uint16_t GetPriority() const { return m_priority; }

        Handle Add(Snapshot entry);
        void Modify(Handle handle, Snapshot entry, ModifyOperation operation);
        void Delete(Handle handle);

        void Reset();

        void Push(TimeUnit timestamp);

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
        Integrator(TCollection& collection, HandleDeleter deleter = [](Handle handle) -> void { if constexpr (std::is_pointer_v<Handle>) { delete handle; } });

        void Integrate(Changes::Instance& diff);

    private:
        TCollection& m_collection;
        HandleDeleter m_handleDeleter;
    };

    Changes(HandleCreator creator = []() -> Handle { if constexpr (std::is_pointer_v<Handle>) { return new typename std::remove_reference<decltype(*Handle())>::type; } else { return Handle(); } });

    Changes(Changes const& other) = delete;
    Changes& operator=(Changes const& other) = delete;

    Changes(Changes&& other) = delete;
    Changes& operator=(Changes&& other) = delete;

    ~Changes() = default;

    Instance Clone(uint16_t priority = 0x8000) const;

    void Push(Instance& instance, TimeUnit timestamp);

    Instance Pull(TimeUnit timestamp = TimeUnit::max());

private:
    HandleCreator m_handleCreator;

    std::mutex m_hostMutex;

    struct PushCommand
    {
        PushCommand(Instance& instance, TimeUnit timestamp);
        bool operator>(PushCommand const& other) const;

        Instance instance;
        TimeUnit timestamp;
    };

    using PushPriorityQueue = std::priority_queue<PushCommand, std::vector<PushCommand>, std::greater<PushCommand>>;

    PushPriorityQueue m_pushes;
};

}
}
}

#include <sleipnir/utility/cc/Changes.imp>

#endif // SLEIPNIR_UTILITY_CC_CHANGES_HPP
