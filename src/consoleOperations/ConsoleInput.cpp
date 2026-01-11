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

// FILE: src\consoleOperations\ConsoleInput.cpp
// PURPOSE: Performs operations related to console inputs and history.

// INCLUDE LIBRARIES

#include "ConsoleInput.hpp"
#include "../history/HistoryManager.hpp"

namespace Console
{
    /**
     * @brief Constructs a console input handler.
     *
     * Initializes standard input and output handles and binds
     * the input system to a history manager instance.
     *
     * @param history Reference to the command history manager.
     */
    Input::Input(History::Manager &history)
        : m_history(history)
    {
        m_stdin = GetStdHandle(STD_INPUT_HANDLE);
        m_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    /**
     * @brief Records the cursor X position where the prompt ends.
     *
     * This position is used as a fixed anchor point when redrawing
     * the input line after edits, cursor movement, or history navigation.
     */
    void Input::setPromptStart()
    {
        CONSOLE_SCREEN_BUFFER_INFO info{};
        GetConsoleScreenBufferInfo(m_stdout, &info);
        m_promptStartX = info.dwCursorPosition.X;
    }

    /**
     * @brief Reads a full line of input from the console.
     *
     * Processes raw keyboard events, supports line editing,
     * cursor movement, and history navigation.
     * The function blocks until the Enter key is pressed.
     *
     * @return The complete input line as a wide string.
     */
    std::wstring Input::readLine()
    {
        m_buffer.clear();
        m_cursor = 0;
        m_history.resetNavigation();

        INPUT_RECORD record{};
        DWORD read = 0;

        while (true)
        {
            ReadConsoleInputW(m_stdin, &record, 1, &read);

            if (record.EventType != KEY_EVENT)
                continue;

            const auto &key = record.Event.KeyEvent;
            if (!key.bKeyDown)
                continue;

            handleKeyEvent(key);

            if (key.wVirtualKeyCode == VK_RETURN)
                break;
        }

        DWORD written;
        WriteConsoleW(m_stdout, L"\n", 1, &written, nullptr);
        return m_buffer;
    }

    /**
     * @brief Dispatches a single key event to the appropriate handler.
     *
     * Interprets special keys such as arrows, backspace, and enter,
     * and forwards printable characters for insertion.
     *
     * @param key Windows KEY_EVENT_RECORD describing the key event.
     */
    void Input::handleKeyEvent(const KEY_EVENT_RECORD &key)
    {
        switch (key.wVirtualKeyCode)
        {
        case VK_BACK:
            backspace();
            break;

        case VK_UP:
            historyUp();
            break;

        case VK_DOWN:
            historyDown();
            break;

        case VK_RETURN:
            break;

        case VK_LEFT:
            arrowLeft();
            break;

        case VK_RIGHT:
            arrowRight();
            break;

        default:
            if (key.uChar.UnicodeChar)
                insertChar(key.uChar.UnicodeChar);
            break;
        }
    }

    /**
     * @brief Inserts a character at the current cursor position.
     *
     * Shifts the buffer contents to the right, advances the cursor,
     * and redraws the input line.
     *
     * @param ch Unicode character to insert.
     */
    void Input::insertChar(wchar_t ch)
    {
        m_buffer.insert(m_cursor, 1, ch);
        ++m_cursor;
        redrawLine();
    }

    /**
     * @brief Deletes the character immediately before the cursor.
     *
     * If the cursor is at the beginning of the line, the operation
     * is ignored. Otherwise, the buffer is updated and redrawn.
     */
    void Input::backspace()
    {
        if (m_cursor == 0)
            return;

        m_buffer.erase(m_cursor - 1, 1);
        --m_cursor;
        redrawLine();
    }

    /**
     * @brief Moves the cursor one position to the right.
     *
     * The cursor will not move past the end of the input buffer.
     */
    void Input::arrowRight()
    {
        if (m_cursor < m_buffer.size())
            ++m_cursor;
        redrawLine();
    }

    /**
     * @brief Moves the cursor one position to the left.
     *
     * The cursor will not move past the beginning of the input buffer.
     */
    void Input::arrowLeft()
    {
        if (m_cursor > 0)
            --m_cursor;
        redrawLine();
    }

    /**
     * @brief Replaces the current buffer with the previous history entry.
     *
     * If no previous entry exists, the operation has no effect.
     */
    void Input::historyUp()
    {
        auto prev = m_history.previous();
        if (!prev)
            return;

        m_buffer = *prev;
        m_cursor = m_buffer.size();
        redrawLine();
    }

    /**
     * @brief Replaces the current buffer with the next history entry.
     *
     * If the history is exhausted, the input buffer is cleared.
     */
    void Input::historyDown()
    {
        auto next = m_history.next();
        if (!next)
        {
            m_buffer.clear();
            m_cursor = 0;
        }
        else
        {
            m_buffer = *next;
            m_cursor = m_buffer.size();
        }

        redrawLine();
    }

    /**
     * @brief Redraws the entire input line on the console.
     *
     * Clears the current line starting from the prompt position,
     * writes the buffer contents, and restores the cursor to its
     * logical position within the buffer.
     */
    void Input::redrawLine()
    {
        CONSOLE_SCREEN_BUFFER_INFO info{};
        GetConsoleScreenBufferInfo(m_stdout, &info);

        COORD start = info.dwCursorPosition;
        start.X = m_promptStartX;

        SetConsoleCursorPosition(m_stdout, start);

        DWORD written;
        std::wstring clear(info.dwSize.X - m_promptStartX, L' ');
        WriteConsoleW(
            m_stdout,
            clear.c_str(),
            static_cast<DWORD>(clear.size()),
            &written,
            nullptr);

        SetConsoleCursorPosition(m_stdout, start);
        WriteConsoleW(
            m_stdout,
            m_buffer.c_str(),
            static_cast<DWORD>(m_buffer.size()),
            &written,
            nullptr);

        start.X = m_promptStartX + static_cast<SHORT>(m_cursor);
        SetConsoleCursorPosition(m_stdout, start);
    }

}
