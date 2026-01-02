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

#include "headers/Error.hpp"
#include "headers/Unicode.hpp"

Error makeLastError(const std::string& prefix)
{
    DWORD code = GetLastError();

    if (code == 0)
        return {};

    LPWSTR buffer = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&buffer,
        0,
        nullptr
    );

    std::string msg = unicode::utf16_to_utf8(buffer);
    LocalFree(buffer);

    if (!prefix.empty())
        msg = prefix + ": " + msg;

    return { code, msg };
}
