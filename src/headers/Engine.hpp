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
#include <vector>

#include "Commands.hpp"
#include "Result.hpp"

class Engine {
    public:
        static void execute(CommandType command, uint8_t flags, const std::vector<std::wstring>& args);

        // COMMAND IMPLEMENTATION           Function prototypes

        static void                         executeEXIT();
        static void                         executeCLEAR();
        static void                         executeSYSTEMINFO();
        static void                         executeSYSTEMSTATS();
        static void                         executeLS(const std::wstring &pathStr);
        static void                         executeLSTree(const std::wstring &pathStr, const std::wstring &prefix);
        static void                         executeREVIEW(const std::wstring &filename);
        
        static bool                         isDirectory(const std::wstring& path);
        static bool                         copyFile(const std::wstring& src, const std::wstring& dst);
        static bool                         copyDirectory(const std::wstring& src, const std::wstring& dst);

        static double                       getCPUUsage();
        static double                       getRAMUsage();
        static double                       getDiskUsage(const std::wstring& drive);
        static double                       getNetworkUsage();

        static Result<std::wstring>         executePWD();
        static Result<std::wstring>         executeWHOAMI();
        static Result<std::wstring>         executeHOSTNAME();
        static Result<std::wstring>         executeDIR(const std::wstring &path);
        static Result<std::wstring>         executeDATETIME();
        static BoolResult                   executeTOUCH(const std::wstring &filename);
        static BoolResult                   executeRM(const std::wstring &path);
        static BoolResult                   executeCD(const std::wstring &path);
        static BoolResult                   executeMKDIR(const std::wstring &dirname);
        static BoolResult                   executeRMDIR(const std::wstring &dirname);
        static BoolResult                   executeMV(const std::wstring& src, const std::wstring& dst);
        static BoolResult                   executeCP(const std::wstring& src, const std::wstring& dst);
        static BoolResult                   executeECHO(const std::vector<std::wstring>& args);

};
