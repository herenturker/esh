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


// ------------------- COMMAND DISPATCHER -------------------
void Engine::execute(CommandType command, uint8_t flags, const std::vector<std::wstring>& args)
{
    auto group = getCommandGroup(command);

    switch (group)
    {
        case CommandGroup::FILE_IO:
            FileIO::FileCommands::execute(command, flags, args);
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

