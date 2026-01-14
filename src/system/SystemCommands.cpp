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

// FILE: src\system\SystemCommands.cpp
// PURPOSE: Executes commands related to system.

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <iostream>

#include <windows.h>
#include <pdh.h>
#include <conio.h>

#pragma comment(lib, "pdh.lib")

#include "../headers/Engine.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "../headers/Unicode.hpp"
#include "../headers/Helper.hpp"
#include "../shell/ShellCommands.hpp"
#include "SystemCommands.hpp"

namespace System
{

    void SystemCommands::execute(CommandType cmd, uint16_t flags, const std::vector<std::wstring> &args)
    {

        switch (cmd)
        {
        case CommandType::SYSTEMINFO:
            // Display system information
            executeSYSTEMINFO();
            break;

        case CommandType::SYSTEMSTATS:
            // Display real-time system statistics
            executeSYSTEMSTATS();
            break;

        default:
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"SystemCommands: Unsupported command" << std::endl;
            console::reset();
            break;
        }
    }

    // SYSTEMINFO COMMAND
    void SystemCommands::executeSYSTEMINFO()
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        auto printSize = [](uintptr_t addr)
        {
            double bytes = static_cast<double>(addr);
            double kb = bytes / 1024.0;
            double mb = kb / 1024.0;
            double gb = mb / 1024.0;
            double tb = gb / 1024.0;
            std::wcout << L"Bytes: " << bytes
                       << L", KB: " << kb
                       << L", MB: " << mb
                       << L", GB: " << gb
                       << L", TB: " << tb << std::endl;
        };

        std::wcout << L"----- System Information -----" << std::endl;
        std::wcout << L"Processor Architecture: ";
        switch (si.wProcessorArchitecture)
        {
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::wcout << L"x64 (AMD or Intel)" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::wcout << L"ARM" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            std::wcout << L"ARM64" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::wcout << L"x86" << std::endl;
            break;
        default:
            std::wcout << L"Unknown" << std::endl;
            break;
        }
        std::wcout << L"Number of Processors: " << si.dwNumberOfProcessors << std::endl;
        std::wcout << L"Page Size: " << si.dwPageSize << L" bytes" << std::endl;

        std::wcout << L"Minimum Application Address: " << si.lpMinimumApplicationAddress << " ";
        printSize(reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress));

        std::wcout << L"Maximum Application Address: " << si.lpMaximumApplicationAddress << " ";
        printSize(reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress));

        std::wcout << L"Active Processor Mask: " << si.dwActiveProcessorMask << std::endl;
        std::wcout << L"Processor Level: " << si.wProcessorLevel << std::endl;
        std::wcout << L"Processor Revision: " << si.wProcessorRevision << std::endl;

        std::wcout << L"--------------------------------" << std::endl;
    }

    double SystemCommands::getCPUUsage()
    {
        static ULONGLONG lastIdle = 0, lastKernel = 0, lastUser = 0;

        FILETIME idle, kernel, user;
        GetSystemTimes(&idle, &kernel, &user);

        ULONGLONG i = ((ULONGLONG)idle.dwHighDateTime << 32) | idle.dwLowDateTime;
        ULONGLONG k = ((ULONGLONG)kernel.dwHighDateTime << 32) | kernel.dwLowDateTime;
        ULONGLONG u = ((ULONGLONG)user.dwHighDateTime << 32) | user.dwLowDateTime;

        ULONGLONG idleDiff = i - lastIdle;
        ULONGLONG totalDiff = (k + u) - (lastKernel + lastUser);

        lastIdle = i;
        lastKernel = k;
        lastUser = u;

        if (totalDiff == 0)
            return 0.0;
        return (1.0 - (double)idleDiff / totalDiff) * 100.0;
    }

    double SystemCommands::getRAMUsage()
    {
        MEMORYSTATUSEX mem{};
        mem.dwLength = sizeof(mem);
        GlobalMemoryStatusEx(&mem);
        return mem.dwMemoryLoad;
    }

    double SystemCommands::getDiskUsage(const std::wstring &drive = L"C:\\")
    {
        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
        if (!GetDiskFreeSpaceExW(drive.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
            return 0.0;

        return 100.0 * (1.0 - static_cast<double>(totalFreeBytes.QuadPart) / static_cast<double>(totalBytes.QuadPart));
    }

    double SystemCommands::getNetworkUsage()
    {
        static PDH_HQUERY query = nullptr;
        static std::vector<PDH_HCOUNTER> counters;
        static bool init = false;
        static double lastValue = 0.0;

        if (!init)
        {
            PdhOpenQuery(nullptr, 0, &query);

            PDH_STATUS status;
            DWORD counterListSize = 0;

            PdhEnumObjectsW(nullptr, nullptr, nullptr, 0, PERF_DETAIL_WIZARD, TRUE);

            PdhAddEnglishCounterW(query,
                                  L"\\Network Interface(*)\\Bytes Total/sec",
                                  0, &counters.emplace_back());

            PdhCollectQueryData(query);
            init = true;
            return 0.0;
        }

        PdhCollectQueryData(query);

        double totalBytesPerSec = 0.0;

        for (auto counter : counters)
        {
            PDH_FMT_COUNTERVALUE value;
            PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &value);
            totalBytesPerSec += value.doubleValue;
        }

        double mbPerSec = totalBytesPerSec / (1024.0 * 1024.0);

        return mbPerSec;
    }

    // SYSTEMSTATS COMMAND
    void SystemCommands::executeSYSTEMSTATS()
    {
        getCPUUsage();
        getNetworkUsage();

        const int refreshIntervalMs = 500; // refresh every 0.5s
        const int inputCheckIntervalMs = 50;

        ULONGLONG lastTime = GetTickCount64();

        while (true)
        {
            const std::vector<std::wstring> arg = {L""};
            ShellCmds::ShellCommands::execute(CommandType::CLEAR, 0, arg);

            double cpu = getCPUUsage();
            double ram = getRAMUsage();
            double disk = getDiskUsage();
            double net = getNetworkUsage();

            console::reset();
            std::wcout << L"----- System Statistics -----" << std::endl;
            console::setColor(ConsoleColor::Cyan);
            std::wcout << L"CPU   " << helper::makeBar(cpu) << std::endl;
            console::setColor(ConsoleColor::Green);
            std::wcout << L"RAM   " << helper::makeBar(ram) << std::endl;
            console::setColor(ConsoleColor::Blue);
            std::wcout << L"DISK  " << helper::makeBar(disk) << std::endl;
            console::setColor(ConsoleColor::Yellow);
            std::wcout << L"NET   " << net << L" MB/s" << std::endl;
            console::reset();
            std::wcout << L"-----------------------------" << std::endl;

            std::wcout << L"\nPress 'q' to quit: " << std::endl;

            ULONGLONG start = GetTickCount64();
            while (GetTickCount64() - start < refreshIntervalMs)
            {
                if (_kbhit())
                {
                    wchar_t q = _getwch();
                    if (q == L'q' || q == L'Q')
                        return;
                }
                Sleep(inputCheckIntervalMs);
            }
        }
    }

}