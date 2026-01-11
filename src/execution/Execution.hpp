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

// FILE: src\execution\Execution.hpp
// PURPOSE: Header file for 'src\execution\Execution.cpp'. Performs redirections and pipelines.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>

#include <windows.h>

#include "../headers/Lexer.hpp"

namespace Execution
{
    class Executor
    {
    public:
        struct Redirection {
            bool hasRedirection;
            std::wstring redirectionString;
        };

        struct RedirectionInfo
        {
            HANDLE stdinHandle  = nullptr;
            HANDLE stdoutHandle = nullptr;
            HANDLE stderrHandle = nullptr;
        };

        enum class RedirectType
        {
            NONE,
            STDIN,
            STDOUT,
            STDERR,
            STDOUT_STDERR
        };

        struct Context
        {
            HANDLE stdinHandle  = INVALID_HANDLE_VALUE;
            HANDLE stdoutHandle = INVALID_HANDLE_VALUE;
            HANDLE stderrHandle = INVALID_HANDLE_VALUE;

            bool pipelineEnabled = false;
            bool redirectionEnabled = false;
        };

        // Entry point from Parser
        static void run(const std::vector<Lexer::Token>& tokens, Context& ctx);

        // Detection
        static bool hasPipeline(const std::vector<Lexer::Token>& tokens);
        static Redirection hasRedirection(const std::vector<Lexer::Token>& tokens);

        // Execution paths
        static void executeSimple(const std::vector<Lexer::Token>& tokens, Context& ctx);
        static void executePipeline(const std::vector<Lexer::Token>& tokens);
        static void executeRedirection(const std::vector<Lexer::Token>& tokens);

        static std::vector<std::vector<Lexer::Token>> splitByPipeline(const std::vector<Lexer::Token>& tokens);

        static RedirectType getRedirectType(const Lexer::Token& t);
        static bool isAppendRedirection(const Lexer::Token& t);

        static HANDLE openFileForWrite(const std::wstring& path, bool append);
        static HANDLE openFileForRead(const std::wstring& path);

        static RedirectionInfo parseRedirections(const std::vector<Lexer::Token>& tokens);

    };
}
