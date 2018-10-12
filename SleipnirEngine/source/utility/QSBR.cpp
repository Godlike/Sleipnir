/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <sleipnir/utility/QSBR.hpp>

#include <limits>
#include <cassert>

namespace sleipnir
{
namespace utility
{

static constexpr QSBR::SectionIndex MaxSectionIndex = std::numeric_limits<QSBR::SectionIndex>::max();

QSBR::QSBR()
    : m_remainingSections(0)
    , m_sectionCount(0)
    , m_nextSection(MaxSectionIndex)
{

}

QSBR::~QSBR()
{
    std::vector<Callback> prevCallbacks;
    std::vector<Callback> curCallbacks;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (0 != m_remainingSections)
        {
            // Log warning
        }

        if (!m_previousInterval.empty())
        {
            m_previousInterval.swap(prevCallbacks);

            // Log warning
        }

        if (!m_currentInterval.empty())
        {
            m_currentInterval.swap(curCallbacks);

            // Log warning
        }
    }

    for (auto const& callback : prevCallbacks)
    {
        callback();
    }

    for (auto const& callback : curCallbacks)
    {
        callback();
    }
}

QSBR::SectionIndex QSBR::RegisterSection()
{
    SectionIndex id;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // increment section count and number of sections to report quiescent state
        ++m_sectionCount;
        ++m_remainingSections;

        // reclaim unused section if available
        if (m_nextSection != MaxSectionIndex)
        {
            id = m_nextSection;

            assert(0 == m_sections[id].active);

            m_nextSection = m_sections[id].next;

            m_sections[id] = SectionEntry();
        }
        // or allocate new section otherwise
        else
        {
            id = m_sections.size();

            m_sections.push_back(SectionEntry());
        }
    }

    return id;
}

void QSBR::ForgetSection(SectionIndex id)
{
    std::vector<Callback> callbacks;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        assert(id < m_sections.size());
        assert(1 == m_sections[id].active);

        // decrement total section count
        --m_sectionCount;

        // if this section did not report quiescent state
        if (0 == m_sections[id].quiescentState)
        {
            // and it is the last to report quiescent state
            if (0 == --m_remainingSections)
            {
                // grab the callbacks
                m_previousInterval.swap(callbacks);
                m_currentInterval.swap(m_previousInterval);

                // reset state for all sections
                for (auto& section : m_sections)
                {
                    section.quiescentState = 0;
                }

                m_remainingSections = m_sectionCount;
            }
        }

        // reset section information
        m_sections[id].active = 0;
        m_sections[id].next = m_nextSection;

        // mark section as unused
        m_nextSection = id;
    }

    // execute grabbed callbacks if any
    for (auto const& callback : callbacks)
    {
        callback();
    }
}

void QSBR::OnQuiescentState(SectionIndex id)
{
    std::vector<Callback> callbacks;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        assert(id < m_sections.size());
        assert(1 == m_sections[id].active);

        // ensure section did not report quiescent state
        if (1 == m_sections[id].quiescentState)
        {
            return;
        }

        m_sections[id].quiescentState = 1;

        // ensure it is the last to report quiescent state
        if (1 == --m_remainingSections)
        {
            return;
        }

        // grab the callbacks
        m_previousInterval.swap(callbacks);
        m_currentInterval.swap(m_previousInterval);

        // reset states for all sections
        for (auto& section : m_sections)
        {
            section.quiescentState = 0;
        }

        m_remainingSections = m_sectionCount;
    }

    // execute grabbed callbacks if any
    for (auto const& callback : callbacks)
    {
        callback();
    }
}

void QSBR::AddCallback(Callback const& callback)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_currentInterval.push_back(callback);
}

}
}
