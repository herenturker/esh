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

// FILE: src\process\ProcessCommands.hpp
// PURPOSE: Header file for 'src\process\ProcessCommands.cpp'. Executes commands related to processes.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>

#include <windows.h>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"

namespace Process
{

    class ProcessCommands
    {
    public:
        static void execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args);

    private:
        // COMMAND IMPLEMENTATION           Function prototypes
        static BoolResult executePS();
        static BoolResult executeKILL(DWORD pid);
    };
}
