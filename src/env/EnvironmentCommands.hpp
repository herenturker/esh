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

// FILE: src\env\EnvironmentCommands.hpp
// PURPOSE: Header file for 'src\env\EnvironmentCommands.cpp'. Executes commands related to environment.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"

namespace Environment {

    class EnvironmentCommands
    {
        public:
            static void execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring>& args);

            // COMMAND IMPLEMENTATION           Function prototypes
            static Result<std::wstring>         executePWD();
            static Result<std::wstring>         executeWHOAMI();
            static Result<std::wstring>         executeHOSTNAME();
            static Result<std::wstring>         executeDATETIME();

        private:
            static BoolResult                   executeENV(); // will add
            static BoolResult                   executeSET(const std::wstring &var, const std::wstring &value); // will add
            static BoolResult                   executeCD(const std::wstring &path);


    };
}