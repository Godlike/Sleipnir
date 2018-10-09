/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef SLEIPNIR_UTILITY_QSBR_HPP
#define SLEIPNIR_UTILITY_QSBR_HPP

#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>

namespace sleipnir
{
namespace utility
{

/** @brief  Quiescent state based reclamation implementation
 *
 *  Reclamation technique based on identifying execution points that lie
 *  outside of all registered critical sections
 *
 *  Read more about this technique:
 *      http://preshing.com/20160726/using-quiescent-states-to-reclaim-memory/
 *
 *  @note   this implementation uses std::mutex as synchronization mechanism
 */
class QSBR
{
public:
    //! Shortcut to section index type
    using SectionIndex = std::size_t;

    //! Shortcut to callback signature
    using Callback = std::function<void()>;

    //! Basic constructor
    QSBR();

    QSBR(const QSBR& other) = delete;
    QSBR& operator=(const QSBR& other) = delete;

    /** @brief  Destructs QSBR object
     *
     *  If there are non-quiescent sections, a warning is issued in the log
     *  If there are queued callbacks, a warning is issued in the log and
     *  callbacks are invoked
     */
    ~QSBR();

    /** @brief  Generates section index
     *
     *  @return section index to be used in quiescent state calls
     */
    SectionIndex RegisterSection();

    /** @brief  Reclaims section for future use in QSBR
     *
     *  @note   SectionIndex @p id is invalidated and cannot be used unless
     *          returned by subsequent RegisterSection() call
     *
     *  Calling this method also means that this section reports quiescent state.
     *  If all sections reported quiescent state, callbacks stored in
     *  @ref m_previousInterval are invoked and @ref m_currentInterval becomes
     *  new @ref m_previousInterval
     */
    void ForgetSection(SectionIndex id);

    /** @brief  Reports that given @p id reached quiescent state
     *
     *  If all sections reported quiescent state, callbacks stored in
     *  @ref m_previousInterval are invoked and @ref m_currentInterval becomes
     *  new @ref m_previousInterval
     *
     *  @param  id  consumer index
     */
    void OnQuiescentState(SectionIndex id);

    /** @brief  Appends callback to @ref m_currentInterval
     *
     *  @param  callback    callback to be invoked when it is safe to do so
     */
    void AddCallback(Callback const& callback);

private:
    //! Describes critical section information
    struct SectionEntry
    {
        //! Flag describing if quiescent state was reported
        uint8_t quiescentState : 1;

        //! Flag describing if this entry is used
        uint8_t active : 1;

        //! Next available section index
        SectionIndex next;

        //! Basic constructor
        SectionEntry() : quiescentState(0), active(1), next(0) {}
    };

    //! Mutex protecting class variables
    std::mutex m_mutex;

    //! Container holding quiescent states of all consumers
    std::vector<SectionEntry> m_sections;

    //! Collection of callbacks for previous interval
    std::vector<Callback> m_previousInterval;

    //! Collection of callbacks for current interval
    std::vector<Callback> m_currentInterval;

    //! Number of active sections that didn't report quiescent state
    SectionIndex m_remainingSections;

    //! Total number of active sections
    SectionIndex m_sectionCount;

    //! Next section id
    SectionIndex m_nextSection;
};

}
}

#endif // SLEIPNIR_UTILITY_QSBR_HPP
