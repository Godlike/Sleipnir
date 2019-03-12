/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_CC_CHANGES_HPP
#define SLEIPNIR_UTILITY_CC_CHANGES_HPP

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

template<typename TMemento>
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

        bool IsEmpty() const;

        bool operator<(Instance const& other) const;

        void Add(Snapshot entry);
        void Modify(Snapshot entry, ModifyOperation operation);
        void Delete(Snapshot entry);

        void Reset();

        void Push();

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

    Instance Clone(uint16_t priority = 0x8000);

    void Push(Instance& instance);

    Instance Pull();

private:
    std::mutex m_hostMutex;

    std::priority_queue<Instance> m_pushes;

};

}
}
}

#include <sleipnir/utility/cc/Changes.imp>

#endif // SLEIPNIR_UTILITY_CC_CHANGES_HPP
