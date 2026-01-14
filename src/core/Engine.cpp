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

// FILE: src\core\Engine.cpp
// PURPOSE: Gets the commands, flags and arguments from 'Parser.cpp' and executes them.

// INCLUDE LIBRARIES

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

#include "../headers/Engine.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "../file/FileCommands.hpp"
#include "../shell/ShellCommands.hpp"
#include "../env/EnvironmentCommands.hpp"
#include "../process/ProcessCommands.hpp"
#include "../system/SystemCommands.hpp"
#include "../execution/Execution.hpp"
#include "../headers/Helper.hpp"

/**
 * @brief Dispatches and executes a parsed command.
 *
 * Determines the command group associated with the given CommandType and
 * forwards execution to the corresponding command handler module
 * (file I/O, process, environment, shell, or system).
 *
 * The execution context is propagated to command handlers that require
 * access to I/O handles, pipeline state, or redirection state.
 *
 * @param command Parsed command identifier.
 * @param flags   Bitmask representing parsed command-line flags.
 * @param args    Vector of command arguments.
 * @param ctx     Execution context carrying redirection and pipeline state.
 */
void Engine::execute(CommandType command, uint16_t flags, const std::vector<std::wstring> &args, Execution::Executor::Context &ctx)
{
    auto group = getCommandGroup(command); // Classifies the command given.

    if (flags & FLAG_HELP)
    {
        helper::showHelp(command);
        return;
    }

    switch (group)
    {
    case CommandGroup::FILE_IO:
        FileIO::FileCommands::execute(command, flags, args, ctx);
        break;

    case CommandGroup::PROCESS:
        Process::ProcessCommands::execute(command, flags, args);
        break;

    case CommandGroup::ENVIRONMENT:
        Environment::EnvironmentCommands::execute(command, flags, args);
        break;

    case CommandGroup::SHELL:
        ShellCmds::ShellCommands::execute(command, flags, args);
        break;

    case CommandGroup::SYSTEM:
        System::SystemCommands::execute(command, flags, args);
        break;

    default:
        console::setColor(ConsoleColor::Red);
        std::wcerr << L"Unknown or unsupported command" << std::endl;
        console::reset();
        break;
    }
}
