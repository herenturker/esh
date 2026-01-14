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

// FILE: src\system\SystemCommands.hpp
// PURPOSE: Header file for 'src\system\SystemCommands.cpp'. Executes commands related to system.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <cstdint>

#include "../headers/Result.hpp"
#include "../headers/Commands.hpp"

/**
 * @brief Namespace containing system-related shell commands.
 */
namespace System
{
    class SystemCommands
    {
    public:
        /**
         * @brief Executes a system command with optional flags and arguments.
         *
         * @param cmd Command type to execute (SYSTEMINFO, SYSTEMSTATS).
         * @param flags Bitwise flags affecting command behavior.
         * @param args Vector of string arguments for the command (currently unused for SYSTEM commands).
         */
        static void execute(CommandType cmd, uint16_t flags, const std::vector<std::wstring> &args);

    private:
        /**
         * @brief Displays static system information (CPU, memory, processor architecture, etc.).
         */
        static void executeSYSTEMINFO();
        /**
         * @brief Continuously displays live system statistics (CPU, RAM, Disk, Network usage).
         *        Updates every 0.5 seconds until 'q' is pressed.
         */
        static void executeSYSTEMSTATS();

        /**
         * @brief Calculates the current CPU usage percentage.
         * @return CPU usage as a double in the range 0.0 - 100.0.
         */
        static double getCPUUsage();

        /**
         * @brief Calculates the current RAM usage percentage.
         * @return RAM usage as a double in the range 0.0 - 100.0.
         */
        static double getRAMUsage();

        /**
         * @brief Calculates disk usage for a given drive.
         * @param drive The drive letter or path to check (default: "C:\\").
         * @return Disk usage as a percentage (0.0 - 100.0). Returns 0.0 on failure.
         */
        static double getDiskUsage(const std::wstring &drive);

        /**
         * @brief Estimates current network usage in MB/s.
         * @return Network usage in megabytes per second.
         */
        static double getNetworkUsage();
    };
}