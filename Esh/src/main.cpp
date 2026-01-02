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

#include <iostream>
#include <string>

#include "headers/Shell.hpp"
#include "headers/Engine.hpp"
#include "headers/ConsoleColor.hpp"
#include "headers/Result.hpp"
#include "headers/Error.hpp"

int main()
{
    // Generic helper to print Result<std::string> or error
    auto printOrError =
    [](const Result<std::string>& res) -> bool
    {
        if (!res.ok())
        {
            console::setColor(ConsoleColor::Red);
            std::cerr << res.error.message << std::endl;
            console::reset();
            return false;
        }

        std::cout << res.value;
        return true;
    };

    try
    {
        std::string json = Shell::load_resource_json(101);
        (void)json; // explicitly unused for now
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::string raw_input;

    while (true)
    {
        /* user@host */
        console::setColor(ConsoleColor::Blue);

        if (!printOrError(Engine::executeWHOAMI())) continue;
        std::cout << "@";
        if (!printOrError(Engine::executeHOSTNAME())) continue;
        std::cout << " ";

        console::reset();

        /* current working directory */
        console::setColor(ConsoleColor::Cyan);

        if (!printOrError(Engine::executePWD())) continue;

        console::reset();

        std::cout << " $ ";

        std::getline(std::cin, raw_input);
        std::cout << std::endl;

        Shell::handleRawInput(raw_input);
    }

    return 0;
}
