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

#pragma once

#include <windows.h>
#include <string>

enum class ConsoleColor : WORD
{
    Default = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
    Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    Gray = FOREGROUND_INTENSITY
};

namespace console
{

    inline void setColor(ConsoleColor color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
    }

    inline void reset()
    {
        setColor(ConsoleColor::Default);
    }

    inline void write(const std::wstring& text)
    {
        DWORD written;
        WriteConsoleW(
            GetStdHandle(STD_OUTPUT_HANDLE),
            text.c_str(),
            static_cast<DWORD>(text.size()),
            &written,
            nullptr
        );
    }

    inline void writeln(const std::wstring& text)
    {
        write(text);
        write(L"\n");
    }

}
