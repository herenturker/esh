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

// FILE: src\core\Error.cpp
// PURPOSE: Creates error messages.

// INCLUDE LIBRARIES

#include "../headers/Error.hpp"
#include "../headers/Unicode.hpp"

/**
 * @brief Constructs an Error object from the last Windows error code.
 *
 * Retrieves the error code returned by GetLastError() and converts it into
 * a human-readable wide-string message using the Windows FormatMessage API.
 *
 * If no error has occurred (error code is zero), an empty Error object is
 * returned.
 *
 * An optional prefix can be prepended to the generated error message to
 * provide additional context (e.g., the failing operation name).
 *
 * @param prefix Optional message prefix to be prepended to the system error text.
 * @return Error object containing the error code and formatted message.
 */
Error makeLastError(const std::wstring &prefix)
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
        nullptr);

    std::wstring message;

    if (buffer)
    {
        message = buffer;
        LocalFree(buffer);
    }

    if (!prefix.empty())
        message = prefix + L": " + message;

    return {code, message};
}
