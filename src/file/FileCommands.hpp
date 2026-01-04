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

#include <vector>
#include <string>
#include <cstdint>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"

namespace FileIO {

    class FileCommands
    {
        public:
            static void execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring>& args);

        private:
            // COMMAND IMPLEMENTATION           Function prototypes

            static void                         executeLS(const std::wstring &pathStr, uint8_t flags, const std::wstring& prefix = L"");
            static void                         executeREW(const std::wstring &filename);
            
            static bool                         isDirectory(const std::wstring& path);
            static bool                         copyFile(const std::wstring& src, const std::wstring& dst);
            static bool                         copyDirectory(const std::wstring& src, const std::wstring& dst);
            static void                         printLsEntry(const WIN32_FIND_DATAW &f, const std::wstring &prefix, uint8_t flags);

            static Result<std::wstring>         executeSTATS(const std::wstring &filename);
            static Result<std::wstring>         executeHEAD(const std::wstring &filename, size_t lineCount);
            static Result<std::wstring>         executeTAIL(const std::wstring &filename, size_t lineCount);

            static BoolResult                   executeTOUCH(const std::wstring &filename);
            static BoolResult                   executeRM(const std::wstring &path);
            static BoolResult                   executeMKDIR(const std::wstring &dirname);
            static BoolResult                   executeRMDIR(const std::wstring &dirname);
            static BoolResult                   executeMV(const std::wstring& src, const std::wstring& dst);
            static BoolResult                   executeCP(const std::wstring& src, const std::wstring& dst);
            static BoolResult                   executeATTRIB(const std::wstring &path, const std::wstring &attributes);

    };
}