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

// FILE: src\shell\ShellCommands.cpp
// PURPOSE: Executes commands related to shell.

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include <Windows.h>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Helper.hpp"
#include "../system/SystemCommands.hpp"
#include "../history/HistoryManager.hpp"
#include "ShellCommands.hpp"

namespace ShellCmds
{ // DO NOT CHANGE the name to 'Shell'. It creates errors (because I tried before).

    void ShellCommands::execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args)
    {
        switch (cmd)
        {
        case CommandType::EXIT:
            // Exit shell immediately
            executeEXIT();
            break;

        case CommandType::CLEAR:
            // Clear the esh console screen
            executeCLEAR();
            break;

        case CommandType::ECHO:
            // Echo the args
            executeECHO(args);
            break;

        default:
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"ShellCommands: Unsupported command" << std::endl;
            console::reset();
            break;
        }
    }

    // CLEAR COMMAND
    void ShellCommands::executeCLEAR()
    {
        std::wcout << L"\x1b[3J\x1b[H" << std::flush;

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hOut, &csbi))
            return;

        SHORT width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        SHORT height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        SMALL_RECT tinyWindow = {0, 0, 0, 0};
        SetConsoleWindowInfo(hOut, TRUE, &tinyWindow);

        COORD tinyBuffer = {width, 1};
        SetConsoleScreenBufferSize(hOut, tinyBuffer);

        COORD newBuffer = {width, height};
        SetConsoleScreenBufferSize(hOut, newBuffer);

        SMALL_RECT newWindow = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
        SetConsoleWindowInfo(hOut, TRUE, &newWindow);

        SetConsoleCursorPosition(hOut, {0, 0});

        DWORD written;
        FillConsoleOutputCharacterW(
            hOut,
            L' ',
            width * height,
            {0, 0},
            &written);

        FillConsoleOutputAttribute(
            hOut,
            csbi.wAttributes,
            width * height,
            {0, 0},
            &written);

        std::wcout << L"\x1b[3J\x1b[H" << std::flush;
    }

    // ECHO COMMAND
    BoolResult ShellCommands::executeECHO(const std::vector<std::wstring> &args)
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return {false, makeLastError(L"echo")};

        bool newline = true;
        size_t start = 0;

        if (!args.empty() && args[0] == L"-n")
        {
            newline = false;
            start = 1;
        }

        std::wstring output;

        for (size_t i = start; i < args.size(); ++i)
        {
            std::wstring w = args[i];
            w = helper::process_escapes(w);

            output += w;
            if (i + 1 < args.size())
                output += L" ";
        }

        if (newline)
            output += L"\n";

        DWORD written;
        WriteConsoleW(
            hOut,
            output.c_str(),
            static_cast<DWORD>(output.size()),
            &written,
            nullptr);

        return {true, {}};
    }

    void ShellCommands::executeEXIT()
    {
        console::setColor(ConsoleColor::Yellow);
        console::writeln(L"Exiting esh...");
        console::reset();
        History::Manager::shutdown();

        exit(EXIT_SUCCESS);
    }
}