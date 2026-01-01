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
#include <windows.h>
#include <string>

#include "headers/Engine.hpp"
#include "headers/Commands.hpp"

void Engine::execute(CommandType command, uint8_t flags) {

    // Implementation of command execution logic
    switch (command) {
        
        case CommandType::LS:
            // Execute ls command
            break;

        case CommandType::PWD:
            // Execute pwd command
            std::cout << executePWD() << std::endl;
            break;

        case CommandType::EXIT:
            // Execute exit
            executeEXIT();
            break;

        case CommandType::CD:
            // Execute cd
            break;

        default:
            // Handle unknown command
            std::cerr << "Unknown command" << std::endl;
            break;
    }
}

std::string Engine::executePWD() {
    DWORD length = GetCurrentDirectoryA(0, nullptr);
    if (length == 0) return ""; // Return empty string on failure

    std::vector<char> buffer(length);
    if (GetCurrentDirectoryA(length, buffer.data()) == 0) return ""; // Return empty string on failure

    return std::string(buffer.data());
}


void Engine::executeEXIT() {
    std::cout << "Exiting shell..." << std::endl;
    ExitProcess(0);
}
