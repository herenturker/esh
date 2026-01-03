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

#include <stdexcept>
#include <iostream>

#include "headers/Commands.hpp"
#include "headers/Shell.hpp"
#include "external/json.hpp"

using json = nlohmann::json;

bool Commands::isBuiltInCommand(const std::wstring &command)
{
    try
    {
        std::string json_text = Shell::load_resource_json(101);
        json j = json::parse(json_text);

        const auto &builtins = j.at("commands").at("builtin");

        return (builtins.contains(std::string(command.begin(), command.end())) ? true : false);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading built-in commands: "
                  << e.what() << std::endl;

        exit(EXIT_FAILURE);
        return false;
    }
}
