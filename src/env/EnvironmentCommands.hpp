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

namespace Environment
{

    /**
     * @brief Dispatches and executes environment-related commands.
     *
     * Determines which environment command is requested and forwards
     * execution to the corresponding implementation. Handles command
     * arguments and flags at a high level.
     *
     * @param cmd   Parsed command type.
     * @param flags Command flags (reserved for future use).
     * @param args  Command arguments.
     */
    class EnvironmentCommands
    {
    public:
    
        /**
         * @brief Executes an environment command.
         *
         * Acts as the main entry point for all environment-related
         * commands such as pwd, whoami, hostname, datetime, and cd.
         *
         * @param cmd   Command type to execute.
         * @param flags Command flags.
         * @param args  Arguments passed to the command.
         */
        static void execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args);

        /**
         * @brief Returns the current working directory.
         *
         * Queries the operating system for the absolute path of the
         * current process working directory.
         *
         * @return Result containing the directory path or an error.
         */
        static Result<std::wstring> executePWD();

        /**
         * @brief Returns the name of the current user.
         *
         * Retrieves the username associated with the current
         * process security context.
         *
         * @return Result containing the username or an error.
         */
        static Result<std::wstring> executeWHOAMI();

        /**
         * @brief Returns the hostname of the system.
         *
         * Obtains the local machine name from the operating system.
         *
         * @return Result containing the hostname or an error.
         */
        static Result<std::wstring> executeHOSTNAME();

        /**
         * @brief Returns the current local date and time.
         *
         * Formats the local system time into a human-readable
         * string representation.
         *
         * @return Result containing the formatted date-time string.
         */
        static Result<std::wstring> executeDATETIME();

    private:
        /**
         * @brief Changes the current working directory.
         *
         * Attempts to set the process working directory to the
         * specified path.
         *
         * @param path Target directory path.
         * @return BoolResult indicating success or failure with error details.
         */
        static BoolResult executeCD(const std::wstring &path);
    };
}
