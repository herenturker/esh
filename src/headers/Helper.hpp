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

// FILE: src\headers\Helper.hpp
// PURPOSE: Helper functions for the engine.

#pragma once

// INCLUDE LIBRARIES

#include <string>
#include <sstream>

#include <iomanip>
#include <windows.h>

#include "Console.hpp"

namespace helper {

    inline std::wstring basename(const std::wstring &path)
    {
        size_t pos = path.find_last_of(L"\\/");
        return (pos == std::wstring::npos)
                ? path
                : path.substr(pos + 1);
    }

    inline std::wstring makeBar(double percent, int width = 30) // for instance: [/////////////           ] %60
    {
        int filled = static_cast<int>((percent / 100.0) * width);
        std::wstring bar = L"[";
        for (int i = 0; i < width; ++i)
            bar += (i < filled ? L'/' : L' ');
        bar += L"] ";
        bar += std::to_wstring(static_cast<int>(percent)) + L"%";
        return bar;
    }

    inline std::wstring process_escapes(const std::wstring &input)
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

    inline static void setColorByAttributes(DWORD attrs)
    {
        if (attrs & FILE_ATTRIBUTE_DIRECTORY)
            console::setColor(ConsoleColor::Blue);

        else if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
            console::setColor(ConsoleColor::Cyan);

        else if (attrs & FILE_ATTRIBUTE_HIDDEN)
            console::setColor(ConsoleColor::Gray);

        else if (attrs & FILE_ATTRIBUTE_READONLY)
            console::setColor(ConsoleColor::Yellow);

        else if (attrs & FILE_ATTRIBUTE_SYSTEM)
            console::setColor(ConsoleColor::Red);

        else if (attrs & FILE_ATTRIBUTE_ARCHIVE)
            console::setColor(ConsoleColor::Green);

        else if (attrs & FILE_ATTRIBUTE_TEMPORARY)
            console::setColor(ConsoleColor::Orange);

        else if (attrs & FILE_ATTRIBUTE_COMPRESSED)
            console::setColor(ConsoleColor::Purple);

        else if (attrs & FILE_ATTRIBUTE_ENCRYPTED)
            console::setColor(ConsoleColor::Pink);
        else
            console::setColor(ConsoleColor::Default);
    }

    inline static std::wstring FileTimeToWString(const FILETIME &ft)
    {
        FILETIME ftLocal;
        SYSTEMTIME st;

        FileTimeToLocalFileTime(&ft, &ftLocal);
        FileTimeToSystemTime(&ftLocal, &st);

        std::wstringstream ss;
        ss << std::setfill(L'0')
        << std::setw(4) << st.wYear << L'-'
        << std::setw(2) << st.wMonth << L'-'
        << std::setw(2) << st.wDay << L' '
        << std::setw(2) << st.wHour << L':'
        << std::setw(2) << st.wMinute << L':'
        << std::setw(2) << st.wSecond;

        return ss.str();
    }

    inline static std::wstring attributesToWSTRING(DWORD attrs)
    {
        std::wstring result;

        if (attrs & FILE_ATTRIBUTE_DIRECTORY)
            result += L"Directory ";

        if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
            result += L"Reparse Point ";

        if (attrs & FILE_ATTRIBUTE_HIDDEN)
            result += L"Hidden ";

        if (attrs & FILE_ATTRIBUTE_READONLY)
            result += L"Read-Only ";

        if (attrs & FILE_ATTRIBUTE_SYSTEM)
            result += L"System ";

        if (attrs & FILE_ATTRIBUTE_ARCHIVE)
            result += L"Archive ";

        if (attrs & FILE_ATTRIBUTE_TEMPORARY)
            result += L"Temporary ";

        if (attrs & FILE_ATTRIBUTE_COMPRESSED)
            result += L"Compressed ";

        if (attrs & FILE_ATTRIBUTE_ENCRYPTED)
            result += L"Encrypted ";

        if (attrs & FILE_ATTRIBUTE_NORMAL)
            result += L"Normal ";

        if (result.empty())
            result = L"Unknown";

        if (!result.empty() && result.back() == L' ')
            result.pop_back();

        return result;
    }
}