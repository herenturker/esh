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

// FILE: src\history\HistoryStorage.cpp
// PURPOSE: Loads and saves command history.

// INCLUDE LIBRARIES

#include <windows.h>

#include "HistoryStorage.hpp"
#include "../platform/AppDataPath.hpp"

namespace HistoryStorage
{
    
    std::vector<std::wstring> load()
    {
        std::vector<std::wstring> result;

        std::wstring path = (Platform::getBasePath() / L"history.txt").wstring();

        HANDLE hFile = CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile == INVALID_HANDLE_VALUE)
            return result;

        DWORD size = GetFileSize(hFile, nullptr);
        if (size == 0)
        {
            CloseHandle(hFile);
            return result;
        }

        std::wstring buffer(size / sizeof(wchar_t), L'\0');
        DWORD read;
        ReadFile(hFile, buffer.data(), size, &read, nullptr);
        CloseHandle(hFile);

        size_t pos = 0;
        while ((pos = buffer.find(L'\n')) != std::wstring::npos)
        {
            std::wstring line = buffer.substr(0, pos);
            if (!line.empty())
                result.push_back(line);
            buffer.erase(0, pos + 1);
        }

        return result;
    }

    void save(const std::vector<std::wstring>& entries)
    {
        std::wstring path = (Platform::getBasePath() / L"history.txt").wstring();

        HANDLE hFile = CreateFileW(
            path.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile == INVALID_HANDLE_VALUE)
            return;

        for (const auto& e : entries)
        {
            DWORD written;
            std::wstring line = e + L"\n";
            WriteFile(
                hFile,
                line.c_str(),
                static_cast<DWORD>(line.size() * sizeof(wchar_t)),
                &written,
                nullptr
            );
        }

        FlushFileBuffers(hFile); 
        CloseHandle(hFile);
    }
}
