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

#include <windows.h>

#include "headers/Engine.hpp"
#include "headers/Commands.hpp"
#include "headers/Unicode.hpp"
#include "headers/ConsoleColor.hpp"
#include "headers/Error.hpp"
#include "headers/Result.hpp"

// helper function
std::wstring basename(const std::wstring& path)
{
    size_t pos = path.find_last_of(L"\\/");
    return (pos == std::wstring::npos)
        ? path
        : path.substr(pos + 1);
}

void Engine::execute(CommandType command, uint8_t flags, const std::vector<std::string> &args)
{
    // Helper lambda to print boolean command results
    // This is the ONLY place where colored output is handled
    auto printBoolResult =
        [](const BoolResult &res, std::string_view successMsg)
    {
        if (res.ok())
        {
            console::setColor(ConsoleColor::Green);
            std::cout << successMsg << std::endl;
        }
        else
        {
            console::setColor(ConsoleColor::Red);
            std::cerr << res.error.message << std::endl;
        }
        console::reset();
    };

    // Implementation of command execution logic
    switch (command)
    {

    case CommandType::PWD:
        // Print working directory
        std::cout << executePWD().value << std::endl;
        break;

    case CommandType::EXIT:
        // Exit shell immediately
        console::setColor(ConsoleColor::Yellow);
        std::cout << "Exiting shell..." << std::endl;
        console::reset();
        ExitProcess(0);

    case CommandType::WHOAMI:
        // Print current user name
        std::cout << executeWHOAMI().value << std::endl;
        break;

    case CommandType::HOSTNAME:
        // Print host machine name
        std::cout << executeHOSTNAME().value << std::endl;
        break;

    case CommandType::DIR:
        // List directory contents (current directory if no argument)
        std::cout << executeDIR(args.empty() ? "." : args[0]).value;
        break;

    case CommandType::DATETIME:
        // Print local date and time
        std::cout << executeDATETIME().value << std::endl;
        break;

    case CommandType::TOUCH:
        // Create a new empty file
        printBoolResult(
            executeTOUCH(args.empty() ? "" : args[0]),
            "File created.");
        break;

    case CommandType::RM:
        // Remove a file
        printBoolResult(
            executeRM(args.empty() ? "" : args[0]),
            "File deleted.");
        break;

    case CommandType::CD:
        // Change current working directory
        printBoolResult(
            executeCD(args.empty() ? "" : args[0]),
            "Directory changed.");
        break;

    case CommandType::MKDIR:
        // Create a new directory
        printBoolResult(
            executeMKDIR(args.empty() ? "" : args[0]),
            "Directory created.");
        break;

    case CommandType::RMDIR:
        // Remove an empty directory
        printBoolResult(
            executeRMDIR(args.empty() ? "" : args[0]),
            "Directory removed.");
        break;

    case CommandType::CLEAR:
        // Clear the Esh console screen
        executeCLEAR();
        break;

    case CommandType::MV:
    {
        // Move (rename) a file or directory
        if (args.size() < 2)
        {
            printBoolResult(
                {false, {0, "Usage: mv <source> <destination>"}},
                "");
            break;
        }

        printBoolResult(
            executeMV(args[0], args[1]),
            "Move operation successful.");
        break;
    }

    case CommandType::CP:
    {
        // Copy a file or directory
        if (args.size() < 2)
        {
            printBoolResult(
                {false, {0, "Usage: cp <source> <destination>"}},
                "");
            break;
        }

        printBoolResult(
            executeCP(args[0], args[1]),
            "Copy operation successful.");
        break;
    }

    default:
        // Unknown or unsupported command
        console::setColor(ConsoleColor::Red);
        std::cerr << "Unknown command" << std::endl;
        console::reset();
        break;
    }
}

// ------------------- COMMAND IMPLEMENTATION -------------------

// PWD COMMAND
Result<std::string> Engine::executePWD()
{
    DWORD length = GetCurrentDirectoryW(0, nullptr);
    if (length == 0)
        return {"", makeLastError("pwd")};

    std::vector<wchar_t> buffer(length);
    if (GetCurrentDirectoryW(length, buffer.data()) == 0)
        return {"", makeLastError("pwd")};

    return {unicode::utf16_to_utf8(buffer.data()), {}};
}

// WHOAMI COMMAND
Result<std::string> Engine::executeWHOAMI()
{
    DWORD length = 0;
    GetUserNameW(nullptr, &length);

    std::vector<wchar_t> buffer(length);
    if (!GetUserNameW(buffer.data(), &length))
        return {"", makeLastError("whoami")};

    return {unicode::utf16_to_utf8(buffer.data()), {}};
}

// HOSTNAME COMMAND
Result<std::string> Engine::executeHOSTNAME()
{
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    if (!GetComputerNameW(buffer, &size))
        return {"", makeLastError("hostname")};

    return {unicode::utf16_to_utf8(std::wstring(buffer, size)), {}};
}

// DATETIME COMMAND
Result<std::string> Engine::executeDATETIME()
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char buffer[64];
    snprintf(buffer, sizeof(buffer),
             "%04d-%02d-%02d %02d:%02d:%02d",
             st.wYear, st.wMonth, st.wDay,
             st.wHour, st.wMinute, st.wSecond);

    return {buffer, {}};
}

// DIR COMMAND
Result<std::string> Engine::executeDIR(const std::string &path)
{
    std::wstring wSearchPath =
        unicode::utf8_to_utf16(path + "\\*");

    WIN32_FIND_DATAW ffd;
    HANDLE hFind = FindFirstFileW(wSearchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
        return {"", makeLastError("dir")};

    std::string result;

    do
    {
        std::string name =
            unicode::utf16_to_utf8(ffd.cFileName);

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            result += name + "/\n";
        else
            result += name + "\n";

    } while (FindNextFileW(hFind, &ffd));

    FindClose(hFind);
    return {result, {}};
}

// TOUCH COMMAND
BoolResult Engine::executeTOUCH(const std::string &filename)
{
    std::wstring wFilename =
        unicode::utf8_to_utf16(filename);

    HANDLE hFile = CreateFileW(
        wFilename.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
        return {false, makeLastError("touch")};

    CloseHandle(hFile);
    return {true, {}};
}

// RM COMMAND
BoolResult Engine::executeRM(const std::string &path)
{
    if (!DeleteFileW(unicode::utf8_to_utf16(path).c_str()))
        return {false, makeLastError("rm")};

    return {true, {}};
}

// CD COMMAND
BoolResult Engine::executeCD(const std::string &path)
{
    if (path.empty())
        return {false, {0, "cd: missing operand"}};

    if (!SetCurrentDirectoryW(unicode::utf8_to_utf16(path).c_str()))
        return {false, makeLastError("cd")};

    return {true, {}};
}

// MKDIR COMMAND
BoolResult Engine::executeMKDIR(const std::string &dirname)
{
    if (!CreateDirectoryW(
            unicode::utf8_to_utf16(dirname).c_str(),
            nullptr))
        return {false, makeLastError("mkdir")};

    return {true, {}};
}

// RMDIR COMMAND
BoolResult Engine::executeRMDIR(const std::string &dirname)
{
    if (!RemoveDirectoryW(
            unicode::utf8_to_utf16(dirname).c_str()))
        return {false, makeLastError("rmdir")};

    return {true, {}};
}

void Engine::executeCLEAR()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return;

    SHORT width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    SHORT height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;

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

BoolResult Engine::executeMV(const std::string& src,
                             const std::string& dst)
{
    std::wstring wSrc = unicode::utf8_to_utf16(src);
    std::wstring wDst = unicode::utf8_to_utf16(dst);

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
        return {false, makeLastError("mv")};
    }

    return {true, {}};
}

BoolResult Engine::executeCP(const std::string& src,
                             const std::string& dst)
{
    std::wstring wSrc = unicode::utf8_to_utf16(src);
    std::wstring wDst = unicode::utf8_to_utf16(dst);

    DWORD attr = GetFileAttributesW(wSrc.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return {false, makeLastError("cp")};

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
        return {false, makeLastError("cp")};

    return {true, {}};
}
