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

// FILE: src\file\FileCommands.hpp
// PURPOSE: Header file for 'src\file\FileCommands.cpp'. Executes commands related to file operations and IO.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"
#include "../execution/Execution.hpp"

namespace FileIO
{

    class FileCommands
    {
    public:
        static void execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args, Execution::Executor::Context&);

    private:
        // COMMAND IMPLEMENTATION           Function prototypes
        static void executeLS(const std::wstring &pathStr, uint8_t flags, const std::wstring &prefix, Execution::Executor::Context&);
        static void executeREW(const std::wstring &filename, Execution::Executor::Context&);
        static void executeSTATS(const std::wstring &filename, Execution::Executor::Context&);
        
        static bool isDirectory(const std::wstring &path);
        static bool copyFile(const std::wstring &src, const std::wstring &dst);
        static bool copyDirectory(const std::wstring &src, const std::wstring &dst);

        static BoolResult executeHEAD(const std::wstring &filename, size_t lineCount, Execution::Executor::Context&);
        static BoolResult executeTAIL(const std::wstring &filename, size_t lineCount, Execution::Executor::Context&);
        static BoolResult executeTOUCH(const std::wstring &filename);
        static BoolResult executeRM(const std::wstring &path);
        static BoolResult executeMKDIR(const std::wstring &dirname);
        static BoolResult executeRMDIR(const std::wstring &dirname);
        static BoolResult executeMV(const std::wstring &src, const std::wstring &dst);
        static BoolResult executeCP(const std::wstring &src, const std::wstring &dst);
    };
}