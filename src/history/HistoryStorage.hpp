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

// FILE: src\history\HistoryStorage.hpp
// PURPOSE: Header file for 'src\history\HistoryStorage.cpp'. Loads and saves command history.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>

namespace HistoryStorage
{
    /**
     * @brief Loads the shell command history from disk.
     *
     * Reads the history file from the application's base path and
     * returns each line as a separate entry in a vector.
     *
     * @return std::vector<std::wstring> Vector of all history entries.
     *         Returns an empty vector if the file does not exist or is empty.
     */
    std::vector<std::wstring> load();

    /**
     * @brief Saves the shell command history to disk.
     *
     * Writes all provided entries into the history file, one per line.
     * Any existing history in the file is overwritten.
     *
     * @param entries Vector of strings representing the history to save.
     */
    void save(const std::vector<std::wstring> &entries);
}