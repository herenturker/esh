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

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <unordered_map>

#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include <psapi.h>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "ProcessCommands.hpp"

namespace Process {
        
    void ProcessCommands::execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args)
    {
        // Helper lambda to print boolean command results
        auto printBoolResult =
            [](const BoolResult &res, const std::wstring &successMsg)
        {
            if (res.ok())
            {
                console::setColor(ConsoleColor::Green);
                console::writeln(successMsg);
            }
            else
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << res.error.message << std::endl;
            }
            console::reset();
        };

        switch (cmd)
        {
            case CommandType::PS:
                executePS();
                break;

            case CommandType::KILL:
                if (args.empty())
                {
                    console::setColor(ConsoleColor::Red);
                    std::wcerr << L"Usage: kill <pid>" << std::endl;
                    console::reset();
                    break;
                }
                executeKILL(std::stoul(args[0]));
                break;

            default:
                console::setColor(ConsoleColor::Red);
                std::wcerr << L"ShellCommands: Unsupported command" << std::endl;
                console::reset();
                break;
        }
    }

    BoolResult ProcessCommands::executePS()
    {
        while (true)
        {
            system("cls"); // we could use executeCLEAR() but why make more function calls?

            std::wcout << L"PID     PPID    NAME\n";
            std::wcout << L"----------------------------------------\n";

            HANDLE snapshot = CreateToolhelp32Snapshot(
                TH32CS_SNAPPROCESS,
                0
            );

            if (snapshot == INVALID_HANDLE_VALUE)
                return {false, makeLastError(L"ps")};

            PROCESSENTRY32W entry;
            entry.dwSize = sizeof(PROCESSENTRY32W);

            if (Process32FirstW(snapshot, &entry))
            {
                do
                {
                    std::wcout
                        << entry.th32ProcessID << L"\t"
                        << entry.th32ParentProcessID << L"\t"
                        << entry.szExeFile << L"\n";
                }
                while (Process32NextW(snapshot, &entry));
            }
            else
            {
                CloseHandle(snapshot);
                return {false, makeLastError(L"ps")};
            }

            std::wcout << L"Press 'q' to quit\n";

            CloseHandle(snapshot);

            if (_kbhit())
            {
                wchar_t ch = _getch();
                if (ch == L'q' || ch == L'Q')
                    break;
            }

            Sleep(500); // refresh interval
        }

        return {true, {}};
    }


    BoolResult ProcessCommands::executeKILL(DWORD pid)
    {

        if (pid == 0)
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"kill: invalid pid\n";
            console::reset();
            return { false, { 0, L"kill: invalid pid" } };

        if (pid == GetCurrentProcessId())
            std::wcerr << L"kill: refusing to kill self\n";
            return { false, { 0, L"kill: refusing to kill self" } };

        HANDLE hProcess = ::OpenProcess(
            PROCESS_TERMINATE,
            FALSE,
            pid
        );

        if (hProcess == nullptr)
            return { false, makeLastError(L"kill") };

        if (!TerminateProcess(hProcess, 1))
        {
            CloseHandle(hProcess);
            return { false, makeLastError(L"kill") };
        }

        CloseHandle(hProcess);

        return { true, {} };
    }

}
