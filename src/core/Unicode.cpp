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

// FILE: src\core\Unicode.cpp
// PURPOSE: Converts the given string to UTF16 or UTF8.

// INCLUDE LIBRARIES

#include <string>

#include <windows.h>

#include "../headers/Unicode.hpp"

namespace unicode
{

    std::wstring utf8_to_utf16(const std::string &utf8)
    {
        if (utf8.empty())
            return L"";

        int wideLen = MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.c_str(),
            -1,
            nullptr,
            0);

        if (wideLen == 0)
            return L"";

        std::wstring utf16(wideLen - 1, L'\0');

        MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.c_str(),
            -1,
            utf16.data(),
            wideLen);

        return utf16;
    }

    std::string utf16_to_utf8(const std::wstring &utf16)
    {
        if (utf16.empty())
            return "";

        int utf8Len = WideCharToMultiByte(
            CP_UTF8,
            0,
            utf16.c_str(),
            -1,
            nullptr,
            0,
            nullptr,
            nullptr);

        if (utf8Len == 0)
            return "";

        std::string utf8(utf8Len - 1, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            0,
            utf16.c_str(),
            -1,
            utf8.data(),
            utf8Len,
            nullptr,
            nullptr);

        return utf8;
    }
}