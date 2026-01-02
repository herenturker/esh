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

void Engine::execute(CommandType command, uint8_t flags, std::string executee)
{

    // Helper lambda to print boolean results with color coding
    auto printBoolResult =
        [](bool ok, std::string_view success, std::string_view failure)
    {
        console::setColor(ok ? ConsoleColor::Green : ConsoleColor::Red);
        std::cout << (ok ? success : failure) << std::endl;
        console::reset();
    };

    // Implementation of command execution logic
    switch (command)
    {
        
    case CommandType::PWD:
        // Execute pwd command
        std::cout << executePWD() << std::endl;
        break;

    case CommandType::EXIT:
        // Execute exit
        executeEXIT();
        break;

    case CommandType::WHOAMI:
        // Execute whoami command
        std::cout << executeWHOAMI() << std::endl;
        break;

    case CommandType::HOSTNAME:
        // Execute hostname command
        std::cout << executeHOSTNAME() << std::endl;
        break;

    case CommandType::DIR:
        // Execute dir command
        std::cout << executeDIR(".") << std::endl; // Current directory
        break;

    case CommandType::DATETIME:
        // Execute datetime command
        std::cout << executeDATETIME() << std::endl;
        break;

    case CommandType::TOUCH:
        // Execute touch command
        printBoolResult(
            executeTOUCH(executee),
            "File created.",
            "File creation failed.");
        break;

    case CommandType::RM:
        // Execute rm command
        printBoolResult(
            executeRM(executee),
            "File deleted.",
            "File deletion failed.");

        break;

    case CommandType::CD:
        // Execute cd command
        printBoolResult(
            executeCD(executee),
            "Directory changed.",
            "Directory change failed.");
        break;

    case CommandType::MKDIR:
        // Execute mkdir command
        printBoolResult(
            executeMKDIR(executee),
            "Directory created.",
            "Directory creation failed.");
        break;

    case CommandType::RMDIR:
        // Execute rmdir command
        printBoolResult(
            executeRMDIR(executee),
            "Directory removed.",
            "Directory removal failed.");
        break;

    default:
        // Handle unknown command
        console::setColor(ConsoleColor::Red);
        std::cerr << "Unknown command" << std::endl;
        console::reset();
        break;
    }
}

// ------------------- COMMAND IMPLEMENTATION -------------------

// PWD COMMAND
std::string Engine::executePWD()
{
    DWORD length = GetCurrentDirectoryW(0, nullptr);
    if (length == 0)
        return "";

    std::vector<wchar_t> buffer(length);
    if (GetCurrentDirectoryW(length, buffer.data()) == 0)
        return "";

    return unicode::utf16_to_utf8(buffer.data());
}

// EXIT COMMAND
void Engine::executeEXIT()
{
    console::setColor(ConsoleColor::Yellow);
    std::cout << "Exiting shell..." << std::endl;
    console::reset();
    ExitProcess(0);
}

// WHOAMI COMMAND
std::string Engine::executeWHOAMI()
{
    DWORD length = 0;
    GetUserNameW(nullptr, &length);
    if (length == 0)
        return "";

    std::vector<wchar_t> buffer(length);
    if (!GetUserNameW(buffer.data(), &length))
        return "";

    return unicode::utf16_to_utf8(buffer.data());
}

// DATETIME COMMAND
std::string Engine::executeDATETIME()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    return std::string(buffer);
}

// HOSTNAME COMMAND
std::string Engine::executeHOSTNAME()
{
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];

    if (!GetComputerNameW(buffer, &size))
        return "";

    return unicode::utf16_to_utf8(std::wstring(buffer, size));
}

// DIR COMMAND
std::string Engine::executeDIR(const std::string &path)
{
    std::wstring wPath = unicode::utf8_to_utf16(path);
    std::wstring searchPath = wPath + L"\\*";

    WIN32_FIND_DATAW ffd;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
        return "Directory not found or access denied.";

    std::string result;

    do
    {
        std::string filename = unicode::utf16_to_utf8(ffd.cFileName);

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            result += filename + "  <DIR>\n";
        }
        else
        {
            LARGE_INTEGER filesize;
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;

            result += filename + "  " + std::to_string(filesize.QuadPart) + " bytes\n";
        }

    } while (FindNextFileW(hFind, &ffd));

    FindClose(hFind);
    return result;
}

// TOUCH COMMAND
bool Engine::executeTOUCH(const std::string &filename)
{
    std::wstring wFilename = unicode::utf8_to_utf16(filename);

    HANDLE hFile = CreateFileW(
        wFilename.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false; // File creation failed
    }

    CloseHandle(hFile);
    return true; // File created successfully
}

// RM COMMAND
bool Engine::executeRM(const std::string &path)
{
    std::wstring wPath = unicode::utf8_to_utf16(path);

    if (DeleteFileW(wPath.c_str()))
    {
        return true; // File deleted successfully
    }
    else
    {
        return false; // File deletion failed
    }
}

// CD COMMAND
bool Engine::executeCD(const std::string &path)
{
    if (path.empty())
        return false;

    std::wstring wInput = unicode::utf8_to_utf16(path);

    DWORD required = GetFullPathNameW(
        wInput.c_str(),
        0,
        nullptr,
        nullptr);

    if (required == 0)
        return false;

    std::wstring wFullPath(required, L'\0');

    DWORD written = GetFullPathNameW(
        wInput.c_str(),
        required,
        wFullPath.data(),
        nullptr);

    if (written == 0)
        return false;

    wFullPath.resize(written);

    return SetCurrentDirectoryW(wFullPath.c_str()) != 0;
}

// MKDIR COMMAND
bool Engine::executeMKDIR(const std::string &dirname)
{
    std::wstring wDirname = unicode::utf8_to_utf16(dirname);

    if (CreateDirectoryW(wDirname.c_str(), nullptr))
    {
        return true; // Directory created successfully
    }
    else
    {
        return false; // Directory creation failed
    }
}

// RMDIR COMMAND
bool Engine::executeRMDIR(const std::string &dirname)
{
    std::wstring wDirname = unicode::utf8_to_utf16(dirname);

    if (RemoveDirectoryW(wDirname.c_str()))
    {
        return true; // Directory removed successfully
    }
    else
    {
        return false; // Directory removal failed
    }
}
