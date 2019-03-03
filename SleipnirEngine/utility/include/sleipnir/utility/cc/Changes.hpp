/*
* Copyright (C) 2019 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_CC_CHANGES_HPP
#define SLEIPNIR_UTILITY_CC_CHANGES_HPP

#include <vector>
#include <utility>
#include <mutex>

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
        Instance& operator=(Instance const& other) = delete;

        Instance(Instance&& other) = default;
        Instance& operator=(Instance&& other) = delete;

        void Add(Snapshot entry) { m_add.push_back(entry); }
        void Modify(Snapshot entry, ModifyOperation operation) { m_modify.emplace_back(entry, operation); }
        void Delete(Snapshot entry) { m_delete.push_back(entry); }

        void Reset() { m_add.clear(); m_modify.clear(); m_delete.clear(); }

        void Push() { m_parent.Push(*this); }

    private:
        friend class Changes;

        Instance(Changes& parent) : m_parent(parent) {}

        void Merge(Instance const& other)
        {
            m_add.insert(m_add.cend(), other.m_add.cbegin(), other.m_add.cend());
            m_modify.insert(m_modify.cend(), other.m_modify.cbegin(), other.m_modify.cend());
            m_delete.insert(m_delete.cend(), other.m_delete.cbegin(), other.m_delete.cend());
        }

        Changes& m_parent;

        AddCollection m_add;
        ModifyCollection m_modify;
        DeleteCollection m_delete;
    };

    Changes() : m_host(*this) {}

    Instance Clone()
    {
        return Instance(*this);
    }

    void Push(Instance const& instance)
    {
        std::lock_guard<std::mutex> lock(m_hostMutex);

        m_host.Merge(instance);
    }

    void Pull(AddCollection& adds, ModifyCollection& modifies, DeleteCollection& deletes)
    {
        adds.clear();
        modifies.clear();
        deletes.clear();

        {
            std::lock_guard<std::mutex> lock(m_hostMutex);

            m_host.m_add.swap(adds);
            m_host.m_modify.swap(modifies);
            m_host.m_delete.swap(deletes);
        }
    }

private:
    std::mutex m_hostMutex;

    Instance m_host;

};

}
}
}

#endif // SLEIPNIR_UTILITY_CC_CHANGES_HPP
