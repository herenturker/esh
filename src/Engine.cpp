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

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

#include <iomanip>
#include <windows.h>
#include <pdh.h>
#include <conio.h>

#pragma comment(lib, "pdh.lib")

#include "headers/Engine.hpp"
#include "headers/Commands.hpp"
#include "headers/Unicode.hpp"
#include "headers/Console.hpp"
#include "headers/Error.hpp"
#include "headers/Result.hpp"

// helper functions
std::wstring basename(const std::wstring &path)
{
    size_t pos = path.find_last_of(L"\\/");
    return (pos == std::wstring::npos)
               ? path
               : path.substr(pos + 1);
}

std::wstring makeBar(double percent, int width = 30)
{
    int filled = static_cast<int>((percent / 100.0) * width);
    std::wstring bar = L"[";
    for (int i = 0; i < width; ++i)
        bar += (i < filled ? L'/' : L' ');
    bar += L"] ";
    bar += std::to_wstring(static_cast<int>(percent)) + L"%";
    return bar;
}

std::wstring process_escapes(const std::wstring &input)
{
    std::wstring out;
    out.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] == L'\\' && i + 1 < input.size())
        {
            switch (input[i + 1])
            {
            case L'n':
                out.push_back(L'\n');
                break;
            case L't':
                out.push_back(L'\t');
                break;
            case L'\\':
                out.push_back(L'\\');
                break;
            default:
                out.push_back(input[i]);
                continue;
            }
            ++i;
        }
        else
        {
            out.push_back(input[i]);
        }
    }

    return out;
}

// ------------------- COMMAND DISPATCHER -------------------

void Engine::execute(CommandType command, uint8_t flags, const std::vector<std::wstring> &args)
{
    // Helper lambda to print boolean command results
    auto printBoolResult =
        [](const BoolResult &res, const std::wstring& successMsg)
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

    // Implementation of command execution logic
    switch (command)
    {

    case CommandType::PWD:
        // Print working directory
        console::writeln(executePWD().value);
        break;

    case CommandType::EXIT:
        // Exit shell immediately
        console::setColor(ConsoleColor::Yellow);
        console::writeln(L"Exiting esh...");
        console::reset();
        ExitProcess(0);

    case CommandType::WHOAMI:
        // Print current user name
        console::writeln(executeWHOAMI().value);
        break;

    case CommandType::HOSTNAME:
        // Print host machine name
        console::writeln(executeHOSTNAME().value);
        break;

    case CommandType::DIR:
        // List directory contents (current directory if no argument)
        console::writeln(executeDIR(args.empty() ? L"." : args[0]).value);
        break;

    case CommandType::DATETIME:
        // Print local date and time
        console::writeln(executeDATETIME().value);
        break;

    case CommandType::TOUCH:
        // Create a new empty file
        printBoolResult(
            executeTOUCH(args.empty() ? L"" : args[0]),
            L"File created.");
        break;

    case CommandType::RM:
        // Remove a file
        printBoolResult(
            executeRM(args.empty() ? L"" : args[0]),
            L"File deleted.");
        break;

    case CommandType::CD:
        // Change current working directory
        printBoolResult(
            executeCD(args.empty() ? L"" : args[0]),
            L"Directory changed.");
        break;

    case CommandType::MKDIR:
        // Create a new directory
        printBoolResult(
            executeMKDIR(args.empty() ? L"" : args[0]),
            L"Directory created.");
        break;

    case CommandType::RMDIR:
        // Remove an empty directory
        printBoolResult(
            executeRMDIR(args.empty() ? L"" : args[0]),
            L"Directory removed.");
        break;

    case CommandType::CLEAR:
        // Clear the esh console screen
        executeCLEAR();
        break;

    case CommandType::MV:
    {
        // Move (rename) a file or directory
        if (args.size() < 2)
        {
            makeLastError(L"mv");
            break;
        }

        executeMV(args[0], args[1]);
        console::writeln(L"Move operation successful.");
        break;
    }

    case CommandType::CP:
    {
        // Copy a file or directory
        if (args.size() < 2)
        {
            makeLastError(L"cp");
            break;
        }

        executeCP(args[0], args[1]);
        console::writeln(L"Copy operation successful.");
        break;
    }

    case CommandType::SYSTEMINFO:
        // Display system information
        executeSYSTEMINFO();
        break;

    case CommandType::SYSTEMSTATS:
        // Display real-time system statistics
        executeSYSTEMSTATS();
        break;

    case CommandType::LS:
    {
        // List directory contents

        if (flags & static_cast<uint8_t>(Flag::RECURSIVE))
        {
            std::wstring path = args.empty() ? L"." : args[0];
            executeLSTree(path, L"");
            break;
        }

        std::wstring path = args.empty() ? L"." : args[0];
        executeLS(path);
        break;
    }

    case CommandType::REVIEW:
    {
        // Review the contents of a file
        if (args.empty())
        {
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"Usage: review <file>" << std::endl;
            console::reset();
            break;
        }

        executeREVIEW(args[0]);
        break;
    }

    case CommandType::ECHO:
    {
        executeECHO(args);
        break;
    }

    default:
        // Unknown or unsupported command
        console::setColor(ConsoleColor::Red);
        std::wcerr << L"Unknown command" << std::endl;
        console::reset();
        break;
    }
}

// ------------------- COMMAND IMPLEMENTATION -------------------

// PWD COMMAND
Result<std::wstring> Engine::executePWD()
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
Result<std::wstring> Engine::executeWHOAMI()
{
    DWORD length = 0;
    GetUserNameW(nullptr, &length);

    std::vector<wchar_t> buffer(length);
    if (!GetUserNameW(buffer.data(), &length))
        return {L"", makeLastError(L"whoami")};

    return {std::wstring(buffer.data()), {}};
}

// HOSTNAME COMMAND
Result<std::wstring> Engine::executeHOSTNAME()
{
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    if (!GetComputerNameW(buffer, &size))
        return {L"", makeLastError(L"hostname")};

    return {std::wstring(buffer, size), {}};
}

// DATETIME COMMAND
Result<std::wstring> Engine::executeDATETIME()
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

// DIR COMMAND
Result<std::wstring> Engine::executeDIR(const std::wstring &path)
{
    std::wstring wSearchPath =
        path + L"\\*";

    WIN32_FIND_DATAW ffd;
    HANDLE hFind = FindFirstFileW(wSearchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
        return {L"", makeLastError(L"dir")};

    std::wstring result;

    do
    {
        std::wstring name = ffd.cFileName;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            result += name + L"/\n";
        else
            result += name + L"\n";

    } while (FindNextFileW(hFind, &ffd));

    FindClose(hFind);
    return {result, {}};
}

// TOUCH COMMAND
BoolResult Engine::executeTOUCH(const std::wstring &filename)
{
    std::wstring wFilename = filename;

    HANDLE hFile = CreateFileW(
        wFilename.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
        return {false, makeLastError(L"touch")};

    CloseHandle(hFile);
    return {true, {}};
}

// RM COMMAND
BoolResult Engine::executeRM(const std::wstring &path)
{
    if (!DeleteFileW(path.c_str()))
        return {false, makeLastError(L"rm")};

    return {true, {}};
}

// CD COMMAND
BoolResult Engine::executeCD(const std::wstring &path)
{
    if (path.empty())
        return {false, {0, L"cd: missing operand"}};

    if (!SetCurrentDirectoryW(path.c_str()))
        return {false, makeLastError(L"cd")};

    return {true, {}};
}

// MKDIR COMMAND
BoolResult Engine::executeMKDIR(const std::wstring &dirname)
{
    if (!CreateDirectoryW(
            dirname.c_str(),
            nullptr))
        return {false, makeLastError(L"mkdir")};

    return {true, {}};
}

// RMDIR COMMAND
BoolResult Engine::executeRMDIR(const std::wstring &dirname)
{
    if (!RemoveDirectoryW(
            dirname.c_str()))
        return {false, makeLastError(L"rmdir")};

    return {true, {}};
}

// CLEAR COMMAND
void Engine::executeCLEAR()
{
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

bool Engine::isDirectory(const std::wstring &path)
{
    DWORD attr = GetFileAttributesW(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool Engine::copyFile(const std::wstring &src, const std::wstring &dst)
{
    return CopyFileW(
               src.c_str(),
               dst.c_str(),
               FALSE // overwrite allowed
               ) != 0;
}

bool Engine::copyDirectory(const std::wstring &src, const std::wstring &dst)
{
    CreateDirectoryW(dst.c_str(), nullptr);

    WIN32_FIND_DATAW ffd;
    std::wstring search = src + L"\\*";

    HANDLE hFind = FindFirstFileW(search.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;

    do
    {
        if (wcscmp(ffd.cFileName, L".") == 0 ||
            wcscmp(ffd.cFileName, L"..") == 0)
            continue;

        std::wstring srcPath = src + L"\\" + ffd.cFileName;
        std::wstring dstPath = dst + L"\\" + ffd.cFileName;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!copyDirectory(srcPath, dstPath))
            {
                FindClose(hFind);
                return false;
            }
        }
        else
        {
            if (!CopyFileW(srcPath.c_str(), dstPath.c_str(), FALSE))
            {
                FindClose(hFind);
                return false;
            }
        }

    } while (FindNextFileW(hFind, &ffd));

    FindClose(hFind);
    return true;
}

// MV COMMAND
BoolResult Engine::executeMV(const std::wstring &src,
                             const std::wstring &dst)
{
    std::wstring wSrc = src;
    std::wstring wDst = dst;
    if (isDirectory(wDst))
    {
        wDst += L"\\";
        wDst += basename(wSrc);
    }

    if (!MoveFileExW(
            wSrc.c_str(),
            wDst.c_str(),
            MOVEFILE_REPLACE_EXISTING |
                MOVEFILE_COPY_ALLOWED))
    {
        return {false, makeLastError(L"mv")};
    }

    return {true, {}};
}

// CP COMMAND
BoolResult Engine::executeCP(const std::wstring &src,
                             const std::wstring &dst)
{
    std::wstring wSrc = src;
    std::wstring wDst = dst;
    DWORD attr = GetFileAttributesW(wSrc.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return {false, makeLastError(L"cp")};

    if (isDirectory(wDst))
    {
        wDst += L"\\";
        wDst += basename(wSrc);
    }

    bool ok;
    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        ok = copyDirectory(wSrc, wDst);
    else
        ok = copyFile(wSrc, wDst);

    if (!ok)
        return {false, makeLastError(L"cp")};

    return {true, {}};
}

// SYSTEMINFO COMMAND
void Engine::executeSYSTEMINFO()
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

double Engine::getCPUUsage()
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

double Engine::getRAMUsage()
{
    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    return mem.dwMemoryLoad;
}

double Engine::getDiskUsage(const std::wstring &drive = L"C:\\")
{
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    if (!GetDiskFreeSpaceExW(drive.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
        return 0.0;

    return 100.0 * (1.0 - static_cast<double>(totalFreeBytes.QuadPart) / static_cast<double>(totalBytes.QuadPart));
}

double Engine::getNetworkUsage()
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
void Engine::executeSYSTEMSTATS()
{
    getCPUUsage();
    getNetworkUsage();

    const int refreshIntervalMs = 500; // refresh every 0.5s
    const int inputCheckIntervalMs = 50;

    ULONGLONG lastTime = GetTickCount64();

    while (true)
    {
        Engine::executeCLEAR();

        double cpu = getCPUUsage();
        double ram = getRAMUsage();
        double disk = getDiskUsage();
        double net = getNetworkUsage();

        console::reset();
        std::wcout << L"----- System Statistics -----" << std::endl;
        console::setColor(ConsoleColor::Cyan);
        std::wcout << L"CPU   " << makeBar(cpu) << std::endl;
        console::setColor(ConsoleColor::Green);
        std::wcout << L"RAM   " << makeBar(ram) << std::endl;
        console::setColor(ConsoleColor::Blue);
        std::wcout << L"DISK  " << makeBar(disk) << std::endl;
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

void Engine::executeLS(const std::wstring &pathStr)
{
    std::wstring path(pathStr.begin(), pathStr.end());
    std::wstring searchPath = path + L"\\*";

    WIN32_FIND_DATAW ffd;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"ls: cannot access '" << path << L"'\n";
        return;
    }

    std::vector<WIN32_FIND_DATAW> files;
    do
    {
        files.push_back(ffd);
    } while (FindNextFileW(hFind, &ffd) != 0);
    FindClose(hFind);

    for (const auto &f : files)
    {
        std::wstring name = f.cFileName;
        if (name == L"." || name == L"..")
            continue;

        DWORD attrs = f.dwFileAttributes;

        if (attrs & FILE_ATTRIBUTE_DIRECTORY)
            console::setColor(ConsoleColor::Blue);
        else if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
            console::setColor(ConsoleColor::Cyan);
        else if (attrs & FILE_ATTRIBUTE_HIDDEN)
            console::setColor(ConsoleColor::Gray);
        else
            console::setColor(ConsoleColor::Default);

        LARGE_INTEGER filesize;
        filesize.LowPart = f.nFileSizeLow;
        filesize.HighPart = f.nFileSizeHigh;

        console::write(attrs & FILE_ATTRIBUTE_DIRECTORY ? L"d " : L"- ");
        console::write(std::to_wstring(filesize.QuadPart) + L" ");
        console::writeln(name);


        console::reset();
    }
}

void Engine::executeLSTree(const std::wstring &pathStr, const std::wstring &prefix = L"")
{
    std::wstring path(pathStr.begin(), pathStr.end());
    std::wstring searchPath = path + L"\\*";

    WIN32_FIND_DATAW ffd;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    std::vector<WIN32_FIND_DATAW> files;
    do
    {
        files.push_back(ffd);
    } while (FindNextFileW(hFind, &ffd) != 0);
    FindClose(hFind);

    for (size_t i = 0; i < files.size(); ++i)
    {
        const auto &f = files[i];
        std::wstring name = f.cFileName;
        if (name == L"." || name == L"..")
            continue;

        bool isLast = (i == files.size() - 1);
        std::wstring connector = isLast ? L"|___" : L"|---";

        DWORD attrs = f.dwFileAttributes;
        if (attrs & FILE_ATTRIBUTE_DIRECTORY)
            console::setColor(ConsoleColor::Blue);
        else if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
            console::setColor(ConsoleColor::Cyan);
        else if (attrs & FILE_ATTRIBUTE_HIDDEN)
            console::setColor(ConsoleColor::Gray);
        else
            console::setColor(ConsoleColor::Default);

        console::write(prefix);
        console::write(connector);
        console::writeln(name);
        console::reset();

        if ((attrs & FILE_ATTRIBUTE_DIRECTORY) && name[0] != L'.')
        {
            std::wstring newPrefix = prefix + (isLast ? L"    " : L"|   ");
            executeLSTree(pathStr + L"\\" + name, newPrefix);
        }
    }
}

void Engine::executeREVIEW(const std::wstring &filename)
{
    std::wstring wFilename = filename;

    HANDLE hFile = CreateFileW(
        wFilename.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        console::setColor(ConsoleColor::Red);
        makeLastError(L"review");
        console::write(L"review: cannot open file '" + wFilename + L"'\n");
        makeLastError(L"review");
        console::reset();
        return;
    }

    char buffer[4096];
    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
    {
        std::string chunk(buffer, bytesRead);
        std::wstring w = unicode::utf8_to_utf16(chunk);
        console::write(w);
    }

    CloseHandle(hFile);
}

BoolResult Engine::executeECHO(const std::vector<std::wstring> &args)
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
        w = process_escapes(w);

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
