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

// FILE: src\history\HistoryBuffer.hpp
// PURPOSE: Header file for 'src\history\HistoryBuffer.cpp'. Performs operations related to history buffer.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <optional>
#include <string>

namespace History
{
    class Buffer
    {
        public:
            // Add command to history
            void push(const std::wstring &command);

            // Fetch previous command. (up arrow)
            std::optional<std::wstring> previous();

            // Fetch next command. (down arrow)
            std::optional<std::wstring> next();

            // Reset history position
            void resetNavigation();

            const std::vector<std::wstring> &entries() const;

        private:
            std::vector<std::wstring> m_entries;

            // navigation cursor
            // size() == "the newest + 1" location
            size_t m_cursor = 0;
    };
}