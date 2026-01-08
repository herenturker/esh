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

// FILE: src\headers\Error.hpp
// PURPOSE: Header file for 'src\core\Error.cpp'. Creates error messages in case of a failure.

#pragma once

// INCLUDE LIBRARIES

#include <string>

#include <windows.h>

struct Error
{
    DWORD code = 0;
    std::wstring message;

    bool hasError() const { return code != 0 || !message.empty(); }
};

Error makeLastError(const std::wstring &prefix = L"");
