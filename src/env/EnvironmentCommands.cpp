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

// FILE: src\env\EnvironmentCommands.cpp
// PURPOSE: Executes commands related to environment.

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <iostream>

#include <windows.h>

#include "../headers/Engine.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "../headers/Unicode.hpp"
#include "../headers/Helper.hpp"
#include "EnvironmentCommands.hpp"

namespace Environment
{

    void EnvironmentCommands::execute(CommandType cmd,
                                      uint8_t /*flags*/,
                                      const std::vector<std::wstring> & /*args*/)
    {
        Result<std::wstring> res;

        switch (cmd)
        {
        case CommandType::PWD:
            res = executePWD();
            break;

        case CommandType::WHOAMI:
            res = executeWHOAMI();
            break;

        case CommandType::HOSTNAME:
            res = executeHOSTNAME();
            break;

        case CommandType::DATETIME:
            res = executeDATETIME();
            break;

        default:
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"Unsupported environment command" << std::endl;
            console::reset();
            return;
        }

        if (!res.ok())
        {
            console::setColor(ConsoleColor::Red);
            std::wcerr << res.error.message << std::endl;
            console::reset();
            return;
        }

        console::write(res.value);
    }

    // CD COMMAND
    BoolResult EnvironmentCommands::executeCD(const std::wstring &path)
    {
        if (path.empty())
            return {false, {0, L"cd: missing operand"}};

        if (!SetCurrentDirectoryW(path.c_str()))
            return {false, makeLastError(L"cd")};

        return {true, {}};
    }

    // PWD COMMAND
    Result<std::wstring> EnvironmentCommands::executePWD()
    {
        DWORD length = GetCurrentDirectoryW(0, nullptr);
        if (length == 0)
            return {L"", makeLastError(L"pwd")};

        std::vector<wchar_t> buffer(length);
        if (GetCurrentDirectoryW(length, buffer.data()) == 0)
            return {L"", makeLastError(L"pwd")};
        return {std::wstring(buffer.data()), {}};
    }

    // WHOAMI COMMAND
    Result<std::wstring> EnvironmentCommands::executeWHOAMI()
    {
        DWORD length = 0;
        GetUserNameW(nullptr, &length);

        std::vector<wchar_t> buffer(length);
        if (!GetUserNameW(buffer.data(), &length))
            return {L"", makeLastError(L"whoami")};

        return {std::wstring(buffer.data()), {}};
    }

    // HOSTNAME COMMAND
    Result<std::wstring> EnvironmentCommands::executeHOSTNAME()
    {
        wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

        if (!GetComputerNameW(buffer, &size))
            return {L"", makeLastError(L"hostname")};

        return {std::wstring(buffer, size), {}};
    }

    // DATETIME COMMAND
    Result<std::wstring> EnvironmentCommands::executeDATETIME()
    {
        SYSTEMTIME st;
        GetLocalTime(&st);

        wchar_t buffer[64];
        swprintf(buffer, sizeof(buffer) / sizeof(wchar_t),
                 L"%04d-%02d-%02d %02d:%02d:%02d",
                 st.wYear, st.wMonth, st.wDay,
                 st.wHour, st.wMinute, st.wSecond);

        return {buffer, {}};
    }

}