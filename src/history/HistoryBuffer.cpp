/*
Copyright 2026 Habil Eren Türker

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// FILE: src\history\HistoryBuffer.cpp
// PURPOSE: Performs operations related to history buffer.

// INCLUDE LIBRARIES

#include "HistoryBuffer.hpp"

namespace History
{

    void Buffer::push(const std::wstring& command)
    {
        m_entries.push_back(command);
        m_cursor = m_entries.size(); // always reset to end
    }

    std::optional<std::wstring> Buffer::previous()
    {
        if (m_entries.empty() || m_cursor == 0)
            return std::nullopt;

        --m_cursor;
        return m_entries[m_cursor];
    }

    std::optional<std::wstring> Buffer::next()
    {
        if (m_cursor >= m_entries.size() - 1)
        {
            m_cursor = m_entries.size();
            return std::nullopt;
        }

        ++m_cursor;
        return m_entries[m_cursor];
    }

    void Buffer::resetNavigation()
    {
        m_cursor = m_entries.size();
    }

    const std::vector<std::wstring>& Buffer::entries() const
    {
        return m_entries;
    }

}
