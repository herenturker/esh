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

// FILE: src\consoleOperations\ConsoleInput.hpp
// PURPOSE: Header file for 'src\consoleOperations\ConsoleInput.cpp'. Performs operations related to console inputs and history.

#pragma once

// INCLUDE LIBRARIES

#include <string>

#include <windows.h>

namespace History
{
    class Manager;
}

namespace Console
{
    class Input
    {
    public:
        explicit Input(History::Manager &history);

        void setPromptStart();
        std::wstring readLine();

    private:
        void handleKeyEvent(const KEY_EVENT_RECORD &key);

        void insertChar(wchar_t ch);
        void backspace();

        void historyUp();
        void historyDown();

        void arrowRight();
        void arrowLeft();

        void redrawLine();

    private:
        History::Manager &m_history;

        std::wstring m_buffer;
        size_t m_cursor = 0;

        SHORT m_promptStartX = 0;
        HANDLE m_stdin = nullptr;
        HANDLE m_stdout = nullptr;
    };
}
