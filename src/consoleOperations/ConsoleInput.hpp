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
        /**
         * @brief Creates a console input handler bound to a history manager.
         *
         * Initializes internal state and retrieves standard input/output
         * handles required for low-level console interaction.
         *
         * @param history Reference to the command history manager.
         */
        explicit Input(History::Manager &history);

        /**
         * @brief Stores the cursor X position after the prompt.
         *
         * This position is used as the left boundary when redrawing
         * the input line during editing operations.
         */
        void setPromptStart();

        /**
         * @brief Reads a single line of user input from the console.
         *
         * Supports interactive editing, cursor movement, and
         * command history navigation. Blocks until Enter is pressed.
         *
         * @return The entered line as a wide string.
         */
        std::wstring readLine();

    private:
        /**
         * @brief Handles a raw Windows keyboard event.
         *
         * Dispatches special keys (arrows, backspace, enter)
         * and printable characters to their respective handlers.
         *
         * @param key The KEY_EVENT_RECORD describing the key event.
         */
        void handleKeyEvent(const KEY_EVENT_RECORD &key);

        /**
         * @brief Inserts a character at the current cursor position.
         *
         * Updates the input buffer, advances the cursor,
         * and redraws the input line.
         *
         * @param ch Unicode character to insert.
         */
        void insertChar(wchar_t ch);

        /**
         * @brief Removes the character before the cursor.
         *
         * If the cursor is at position zero, the operation is ignored.
         */
        void backspace();

        /**
         * @brief Replaces the buffer with the previous history entry.
         *
         * If no previous entry exists, the buffer remains unchanged.
         */
        void historyUp();

        /**
         * @brief Replaces the buffer with the next history entry.
         *
         * If history navigation reaches the end, the buffer is cleared.
         */
        void historyDown();

        /**
         * @brief Moves the cursor one position to the right.
         *
         * The cursor will not move beyond the end of the buffer.
         */
        void arrowRight();

        /**
         * @brief Moves the cursor one position to the left.
         *
         * The cursor will not move before the beginning of the buffer.
         */
        void arrowLeft();

        /**
         * @brief Redraws the input line on the console.
         *
         * Clears the current line from the prompt start position,
         * writes the buffer contents, and restores the cursor position.
         */
        void redrawLine();

    private:
        /** Reference to the command history manager. */
        History::Manager &m_history;

        /** Current input buffer. */
        std::wstring m_buffer;

        /** Cursor position within the input buffer. */
        size_t m_cursor = 0;

        /** X-coordinate where the prompt ends. */
        SHORT m_promptStartX = 0;

        /** Handle to the standard input stream. */
        HANDLE m_stdin = nullptr;

        /** Handle to the standard output stream. */
        HANDLE m_stdout = nullptr;
    };
}
