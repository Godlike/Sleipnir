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

        bool IsEmpty() const { return m_add.empty() && m_modify.empty() && m_delete.empty(); }

        bool operator<(Instance const& other) const { return m_priority < other.m_priority; }

        void Add(Snapshot entry) { m_add.push_back(entry); }
        void Modify(Snapshot entry, ModifyOperation operation) { m_modify.emplace_back(entry, operation); }
        void Delete(Snapshot entry) { m_delete.push_back(entry); }

        void Reset() { m_add.clear(); m_modify.clear(); m_delete.clear(); }

        void Push()
        {
            assert(nullptr != m_pParent);

            m_pParent->Push(*this);
            Reset();
        }

        void Export(AddCollection& adds, ModifyCollection& modifies, DeleteCollection& deletes)
        {
            adds.clear();
            modifies.clear();
            deletes.clear();

            m_add.swap(adds);
            m_modify.swap(modifies);
            m_delete.swap(deletes);
        }

    private:
        friend class Changes;

        Instance(Changes* pParent, uint16_t priority = 0x8000) : m_pParent(pParent), m_priority(priority) {}

        void Merge(Instance const& other)
        {
            m_add.insert(m_add.cend(), other.m_add.cbegin(), other.m_add.cend());
            m_modify.insert(m_modify.cend(), other.m_modify.cbegin(), other.m_modify.cend());
            m_delete.insert(m_delete.cend(), other.m_delete.cbegin(), other.m_delete.cend());
        }

        Changes* m_pParent;
        uint16_t m_priority;

        AddCollection m_add;
        ModifyCollection m_modify;
        DeleteCollection m_delete;
    };

    template<typename TChanges, typename Collection>
    class Integrator
    {
    public:
        Integrator(Collection& collection) : m_collection(collection) {}

        void Integrate(typename TChanges::Instance& diff)
        {
            typename TChanges::AddCollection adds;
            typename TChanges::ModifyCollection modifies;
            typename TChanges::DeleteCollection deletes;

            diff.Export(adds, modifies, deletes);

            for (auto const& _delete : deletes)
            {
                m_collection.Delete(_delete);
            }

            for (auto const& _add : adds)
            {
                m_collection.Spawn(_add);
            }

            for (auto const& _modify : modifies)
            {
                Object* pObj = m_collection.Get(_modify.first);

                if (nullptr != pObj)
                {
                    (pObj->*(_modify.second))(_modify.first);
                }
            }
        }

    private:
        Collection& m_collection;
    };

    Changes() {}

    Instance Clone(uint16_t priority = 0x8000)
    {
        return Instance(this, priority);
    }

    void Push(Instance const& instance)
    {
        std::lock_guard<std::mutex> lock(m_hostMutex);

        m_pushes.push(instance);
    }

    Instance Pull()
    {
        std::priority_queue<Instance> pending;

        {
            std::lock_guard<std::mutex> lock(m_hostMutex);

            pending.swap(m_pushes);
        }

        Instance result(nullptr, 0);

        while (!pending.empty())
        {
            result.Merge(pending.top());
            pending.pop();
        }

        return result;
    }

private:
    std::mutex m_hostMutex;

    std::priority_queue<Instance> m_pushes;

};

}
}
}

#endif // SLEIPNIR_UTILITY_CC_CHANGES_HPP
