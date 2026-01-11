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
    /**
     * @class Executor
     * @brief Handles command execution, including pipelines and I/O redirections.
     *
     * This class provides static functions for executing tokenized shell commands,
     * managing pipelines, and handling input/output redirections on Windows.
     */
    class Executor
    {
    public:
        /**
         * @struct Redirection
         * @brief Indicates whether a command contains a redirection token.
         */
        struct Redirection
        {
            bool hasRedirection;            ///< True if any redirection exists
            std::wstring redirectionString; ///< Redirection operator string (>, >>, 2>, etc.)
        };

        /**
         * @struct RedirectionInfo
         * @brief Stores opened HANDLEs for standard input, output, and error.
         */
        struct RedirectionInfo
        {
            HANDLE stdinHandle = nullptr;  ///< Handle for redirected input
            HANDLE stdoutHandle = nullptr; ///< Handle for redirected output
            HANDLE stderrHandle = nullptr; ///< Handle for redirected error
        };

        /**
         * @enum RedirectType
         * @brief Type of redirection a token represents.
         */
        enum class RedirectType
        {
            NONE,         ///< No redirection
            STDIN,        ///< Input redirection '<'
            STDOUT,       ///< Output redirection '>' or '>>'
            STDERR,       ///< Error redirection '2>' or '2>>'
            STDOUT_STDERR ///< Combined output and error '&>' or '&>>'
        };

        /**
         * @struct Context
         * @brief Tracks the current execution context for a command.
         */
        struct Context
        {
            HANDLE stdinHandle = INVALID_HANDLE_VALUE;  ///< Current STDIN handle
            HANDLE stdoutHandle = INVALID_HANDLE_VALUE; ///< Current STDOUT handle
            HANDLE stderrHandle = INVALID_HANDLE_VALUE; ///< Current STDERR handle

            bool pipelineEnabled = false;    ///< True if executing in a pipeline
            bool redirectionEnabled = false; ///< True if any redirection is active
        };

        /**
         * @brief Main entry point for executing a tokenized command.
         *
         * Detects pipelines and redirections, then dispatches the appropriate execution path.
         *
         * @param tokens Tokenized command input.
         * @param ctx    Execution context including standard handles and flags.
         */
        static void run(const std::vector<Lexer::Token> &tokens, Context &ctx);

        /**
         * @brief Checks if a pipeline '|' exists in the token list.
         *
         * @param tokens Tokenized input.
         * @return true if pipeline exists; false otherwise.
         */
        static bool hasPipeline(const std::vector<Lexer::Token> &tokens);

        /**
         * @brief Checks if any redirection tokens exist in the token list.
         *
         * @param tokens Tokenized input.
         * @return Redirection struct with existence flag and operator.
         */
        static Redirection hasRedirection(const std::vector<Lexer::Token> &tokens);

        /**
         * @brief Executes a command without pipelines or redirections.
         *
         * Converts tokens into command, flags, and arguments, and forwards them to Engine::execute.
         *
         * @param tokens Tokenized command input.
         * @param ctx    Execution context.
         */
        static void executeSimple(const std::vector<Lexer::Token> &tokens, Context &ctx);

        /**
         * @brief Executes multiple commands connected via pipeline.
         *
         * Sets up pipes between processes and handles I/O appropriately.
         *
         * @param tokens Tokenized command input containing '|' operators.
         */
        static void executePipeline(const std::vector<Lexer::Token> &tokens);

        /**
         * @brief Splits a token list into separate commands at pipeline tokens.
         *
         * @param tokens Tokenized command input.
         * @return Vector of token vectors for each command in the pipeline.
         */
        static std::vector<std::vector<Lexer::Token>> splitByPipeline(const std::vector<Lexer::Token> &tokens);

        /**
         * @brief Returns the type of redirection a token represents.
         *
         * @param t Token to inspect.
         * @return RedirectType enum.
         */
        static RedirectType getRedirectType(const Lexer::Token &t);

        /**
         * @brief Checks whether a redirection token indicates append mode.
         *
         * @param t Token to inspect.
         * @return true if append redirection (>> or &>>); false otherwise.
         */
        static bool isAppendRedirection(const Lexer::Token &t);

        /**
         * @brief Opens a file handle for writing or appending.
         *
         * @param path   File path to open.
         * @param append true to append, false to overwrite.
         * @return HANDLE to the file or nullptr if failed.
         */
        static HANDLE openFileForWrite(const std::wstring &path, bool append);

        /**
         * @brief Opens a file handle for reading.
         *
         * @param path File path to open.
         * @return HANDLE to the file or nullptr if failed.
         */
        static HANDLE openFileForRead(const std::wstring &path);

        /**
         * @brief Parses redirection tokens and opens file handles accordingly.
         *
         * @param tokens Tokenized command input.
         * @return RedirectionInfo struct containing opened handles for STDIN, STDOUT, STDERR.
         */
        static RedirectionInfo parseRedirections(const std::vector<Lexer::Token> &tokens);
    };
}
