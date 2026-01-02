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

#include <cstdint>
#include <string>

#include "Commands.hpp"

class Engine {
    public:
        static void execute(CommandType command, uint8_t flags, std::string executee);

        // COMMAND IMPLEMENTATION           Function prototypes
        static std::string                  executePWD();
        static void                         executeEXIT();
        static std::string                  executeWHOAMI();
        static std::string                  executeHOSTNAME();
        static std::string                  executeDIR(const std::string &path);
        static std::string                  executeDATETIME();
        static bool                         executeTOUCH(const std::string &filename);
        static bool                         executeRM(const std::string &path);
        static bool                         executeCD(const std::string &path);
        static bool                         executeMKDIR(const std::string &dirname);
        static bool                         executeRMDIR(const std::string &dirname);

};
