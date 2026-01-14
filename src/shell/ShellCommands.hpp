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

// FILE: src\shell\ShellCommands.hpp
// PURPOSE: Header file for 'src\system\ShellCommands.cpp'. Executes commands related to shell.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"

/**
 * @brief Namespace containing shell-related commands.
 */
namespace ShellCmds
{
    /**
     * @brief Implements basic shell commands for the custom shell.
     *
     * Supported commands:
     *  - EXIT: Exit the shell.
     *  - CLEAR: Clear the shell console.
     *  - ECHO: Print arguments to the console.
     */
    class ShellCommands
    {
    public:
        /**
         * @brief Executes a shell command with optional flags and arguments.
         * 
         * @param cmd Command type to execute (EXIT, CLEAR, ECHO).
         * @param flags Bitwise flags affecting command behavior.
         * @param args Vector of string arguments for the command.
         */
        static void execute(CommandType cmd, uint16_t flags, const std::vector<std::wstring> &args);

    private:
        /**
         * @brief Exits the shell immediately.
         */
        static void executeEXIT();

        /**
         * @brief Clears the shell console screen.
         */
        static void executeCLEAR();

        /**
         * @brief Prints the provided arguments to the console.
         * 
         * @param args Vector of strings to print.
         * @return BoolResult indicating success or failure.
         */
        static BoolResult executeECHO(const std::vector<std::wstring> &args);
    };
}
