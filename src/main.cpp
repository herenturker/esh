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

#include <io.h>
#include <fcntl.h>

#include "headers/Shell.hpp"
#include "headers/Engine.hpp"
#include "headers/Console.hpp"
#include "headers/Result.hpp"
#include "headers/Error.hpp"
#include "headers/Unicode.hpp"

int wmain()
{
    _setmode(_fileno(stdin),  _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);

    // Generic helper to print Result<std::wstring> or error
    auto printOrError =
        [](const Result<std::wstring> &res) -> bool
    {
        if (!res.ok())
        {
            console::setColor(ConsoleColor::Red);
            std::wcerr << res.error.message << L"\n";
            console::reset();
            return false;
        }

        console::write(res.value);
        return true;
    };

    try
    {
        std::string json = Shell::load_resource_json(101);
        (void)json; // explicitly unused for now
    }
    catch (const std::exception &e)
    {
        std::wcerr << L"Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::wstring raw_input;

    while (true)
    {
        /* user@host */
        console::setColor(ConsoleColor::Blue);

        if (!printOrError(Engine::executeWHOAMI()))
            continue;
        console::write(L"@");
        if (!printOrError(Engine::executeHOSTNAME()))
            continue;
        console::write(L" ");

        console::reset();

        /* current working directory */
        console::setColor(ConsoleColor::Cyan);

        if (!printOrError(Engine::executePWD()))
            continue;
        console::write(L" ");
        console::reset();

        console::write(L"$ ");

        std::getline(std::wcin, raw_input);
        console::write(L"\n");

        Shell::handleRawInput(raw_input);
    }

    return 0;
}
