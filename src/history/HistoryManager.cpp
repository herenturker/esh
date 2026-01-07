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

#include "HistoryManager.hpp"
#include "HistoryStorage.hpp"

using History::Buffer;
using History::Manager;

namespace History
{

    void Manager::initialize()
    {
        // Load history from disk
        const auto entries = HistoryStorage::load();

        for (const auto &entry : entries)
        {
            m_buffer.push(entry);
        }

        // Cursor must point to "after last"
        m_buffer.resetNavigation();
    }

    void Manager::add(const std::wstring &command)
    {
        if (command.empty())
            return;

        m_buffer.push(command);
    }

    std::optional<std::wstring> Manager::previous()
    {
        return m_buffer.previous();
    }

    std::optional<std::wstring> Manager::next()
    {
        return m_buffer.next();
    }

    void Manager::resetNavigation()
    {
        m_buffer.resetNavigation();
    }

    void Manager::shutdown()
    {
        // Persist RAM history to disk
        HistoryStorage::save(m_buffer.entries());
    }

}
