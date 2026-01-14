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

// FILE: src/core/Commands.cpp
// PURPOSE: Manages and queries built-in shell commands.

// INCLUDE LIBRARIES

#include <unordered_set>
#include <mutex>
#include <iostream>

#include "../headers/Commands.hpp"
#include "../headers/Shell.hpp"
#include "../external/json.hpp"

using json = nlohmann::json;

namespace
{
    std::unordered_set<std::wstring> builtinCommands;
    std::once_flag initFlag;

    /**
     * @brief Loads built-in shell commands from the embedded JSON resource.
     *
     * Parses the JSON resource embedded in the executable and extracts
     * the list of built-in commands defined under `commands.builtin`.
     * Each command name is converted to `std::wstring` and stored in an
     * `std::unordered_set` for fast lookup.
     *
     * This function is intended to be called exactly once and is protected
     * by `std::call_once` to guarantee thread-safe, one-time initialization.
     *
     * On failure, an error message is written to standard error.
     */
    void loadBuiltins()
    {
        try
        {
            std::string json_text = Shell::load_resource_json(101);
            json j = json::parse(json_text);

            const auto &builtins = j.at("commands").at("builtin");

            for (auto it = builtins.begin(); it != builtins.end(); ++it)
            {
                const std::string &cmd = it.key();
                builtinCommands.insert(std::wstring(cmd.begin(), cmd.end()));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[esh] Failed to load built-in commands: "
                      << e.what() << std::endl;
        }
    }
}

/**
 * @brief Checks whether a command is a built-in shell command.
 *
 * Ensures that the built-in command list is initialized, then performs
 * a constant-time lookup to determine whether the given command exists
 * in the built-in command set.
 *
 * @param command The command name to check.
 * @return true if the command is a built-in command, false otherwise.
 * 
 */
bool Commands::isBuiltInCommand(const std::wstring &command)
{
    std::call_once(initFlag, loadBuiltins);

    if (builtinCommands.empty())
        return false;

    return builtinCommands.find(command) != builtinCommands.end();
}

