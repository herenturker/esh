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

void Engine::execute(CommandType command, uint8_t flags, std::string executee)
{

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
        std::cout << (executeTOUCH(executee) ? "File created." : "File creation failed.") << std::endl;
        break;

    case CommandType::RM:
        // Execute rm command
        std::cout << (executeRM(executee) ? "File deleted." : "File deletion failed.") << std::endl;
        break;

    default:
        // Handle unknown command
        std::cerr << "Unknown command" << std::endl;
        break;
    }
}

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

void Engine::executeEXIT()
{
    std::cout << "Exiting shell..." << std::endl;
    ExitProcess(0);
}

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

std::string Engine::executeDATETIME()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    return std::string(buffer);
}

std::string Engine::executeHOSTNAME()
{
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];

    if (!GetComputerNameW(buffer, &size))
        return "";

    return unicode::utf16_to_utf8(std::wstring(buffer, size));
}

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
