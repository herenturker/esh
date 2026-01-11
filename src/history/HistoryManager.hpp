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
    /**
     * @brief Manages shell command history and navigation.
     *
     * Uses an internal Buffer to store commands and provides methods
     * to navigate previous/next entries, add new commands, and persist
     * the history to disk.
     */
    class Manager
    {
    public:
        Manager() = default;

        /**
         * @brief Initializes the history manager.
         *
         * Loads existing history from disk and prepares the buffer
         * for navigation.
         */
        void initialize();

        /**
         * @brief Adds a new command to the history.
         *
         * Ignores empty commands.
         *
         * @param command Command string to add.
         */
        void add(const std::wstring &command);

        /**
         * @brief Returns the previous command in history, if available.
         *
         * Moves the navigation cursor backward.
         *
         * @return std::optional<std::wstring> Previous command or empty if none.
         */
        std::optional<std::wstring> previous();

        /**
         * @brief Returns the next command in history, if available.
         *
         * Moves the navigation cursor forward.
         *
         * @return std::optional<std::wstring> Next command or empty if none.
         */
        std::optional<std::wstring> next();

        /**
         * @brief Resets navigation cursor to after the last command.
         */
        void resetNavigation();

        /**
         * @brief Shuts down the manager and persists history to disk.
         */
        static void shutdown();

        ~Manager()
        {
            shutdown();
        }

    private:
        std::wstring m_historyFile;
        inline static Buffer m_buffer; /**< Internal buffer holding command history */
    };
}
