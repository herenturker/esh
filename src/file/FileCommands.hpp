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
    /**
     * @brief Provides implementations of common file-related commands for the shell.
     *
     * Supported commands include:
     *  - ls, rew, stats
     *  - head, tail
     *  - touch, rm, mkdir, rmdir
     *  - mv, cp
     *
     * This class handles command execution with proper context, flags, and output handling.
     */
    class FileCommands
    {
    public:
    
        /**
         * @brief Executes a file command with the given arguments.
         *
         * @param cmd The type of command to execute (CommandType enum).
         * @param flags Bitwise flags affecting command behavior (e.g., verbose, recursive).
         * @param args Arguments for the command (file/directory names, counts, etc.).
         * @param ctx Execution context containing handles, pipeline state, and redirection state.
         */
        static void execute(CommandType cmd, uint16_t flags, const std::vector<std::wstring> &args, Execution::Executor::Context &ctx);

    private:
        // --------------------
        // Command implementations
        // --------------------

        /**
         * @brief Lists the contents of a directory.
         *
         * @param pathStr Directory path (default is current directory).
         * @param flags Bitwise flags (e.g., recursive, all, verbose).
         * @param prefix Prefix for recursive tree-like output.
         * @param ctx Execution context.
         */
        static void executeLS(const std::wstring &pathStr, uint16_t flags, const std::wstring &prefix, Execution::Executor::Context &ctx);

        /**
         * @brief Reads and writes the contents of a file.
         * @param filename File path.
         * @param ctx Execution context.
         */
        static void executeREW(const std::wstring &filename, Execution::Executor::Context &ctx);

        /**
         * @brief Prints file statistics (lines, words, bytes, size, timestamps, attributes).
         * @param filename File path.
         * @param ctx Execution context.
         */
        static void executeSTATS(const std::wstring &filename, Execution::Executor::Context &ctx);

        /**
         * @brief Checks whether a given path is a directory.
         * @param path Path to check.
         * @return true if the path exists and is a directory, false otherwise.
         */
        static bool isDirectory(const std::wstring &path);

        /**
         * @brief Copies a single file from source to destination.
         * @param src Source file path.
         * @param dst Destination file path.
         * @return true if copy succeeds, false otherwise.
         */
        static bool copyFile(const std::wstring &src, const std::wstring &dst);

        /**
         * @brief Recursively copies a directory.
         * @param src Source directory path.
         * @param dst Destination directory path.
         * @return true if copy succeeds, false otherwise.
         */
        static bool copyDirectory(const std::wstring &src, const std::wstring &dst);

        /**
         * @brief Outputs the first N lines of a file (head command).
         * @param filename File path.
         * @param lineCount Number of lines to output.
         * @param ctx Execution context.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeHEAD(const std::wstring &filename, size_t lineCount, Execution::Executor::Context &ctx);

        /**
         * @brief Outputs the last N lines of a file (tail command).
         * @param filename File path.
         * @param lineCount Number of lines to output.
         * @param ctx Execution context.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeTAIL(const std::wstring &filename, size_t lineCount, Execution::Executor::Context &ctx);

        /**
         * @brief Creates a new empty file (touch command).
         * @param filename File path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeTOUCH(const std::wstring &filename);

        /**
         * @brief Deletes a file (rm command).
         * @param path File path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeRM(const std::wstring &path);

        /**
         * @brief Creates a new directory (mkdir command).
         * @param dirname Directory path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeMKDIR(const std::wstring &dirname);

        /**
         * @brief Removes an empty directory (rmdir command).
         * @param dirname Directory path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeRMDIR(const std::wstring &dirname);

        /**
         * @brief Moves a file or directory to a new location (mv command).
         * @param src Source path.
         * @param dst Destination path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeMV(const std::wstring &src, const std::wstring &dst);

        /**
         * @brief Copies a file or directory (cp command).
         * @param src Source path.
         * @param dst Destination path.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeCP(const std::wstring &src, const std::wstring &dst);

    };
}
