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

// FILE: src\history\HistoryManager.hpp
// PURPOSE: Header file for 'src\history\HistoryManager.cpp'. Performs and manages history buffer operations.

#pragma once

// INCLUDE LIBRARIES

#include <string>

#include "HistoryBuffer.hpp"

namespace History
{
    class Manager
    {
    public:
        Manager() = default;

        // call when program starts
        void initialize();

        // add new command
        void add(const std::wstring& command);

        // navigate
        std::optional<std::wstring> previous();
        std::optional<std::wstring> next();

        void resetNavigation();

        void shutdown();

        ~Manager()
        {
            shutdown();
        }

    private:
        std::wstring m_historyFile;
        Buffer m_buffer;
    };
}
