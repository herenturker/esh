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

// FILE: src\core\Shell.cpp
// PURPOSE: Loads 'esh.json' file, sends input to 'Lexer.cpp' and then sends the result to 'Parser.cpp'

// INCLUDE LIBRARIES

#include <string>
#include <stdexcept>
#include <iostream>

#include <windows.h>

#include "../headers/Shell.hpp"
#include "../headers/Lexer.hpp"
#include "../headers/Token.hpp"
#include "../headers/Parser.hpp"
#include "../execution/Execution.hpp"

/**
 * @brief Processes raw shell input through the lexical and parsing stages.
 *
 * This function serves as the entry point between user-provided raw input
 * and the execution pipeline. It performs no execution itself; instead,
 * it orchestrates the transformation of the input through:
 *
 *  - Tokenization (lexical analysis)
 *  - Parsing and semantic interpretation
 *
 * All resulting execution state is stored within the provided execution
 * context.
 *
 * @param raw_input The raw command line input provided by the user.
 * @param ctx       Execution context that accumulates parsing and execution state.
 */
void Shell::handleRawInput(std::wstring &raw_input, Execution::Executor::Context &ctx)
{
    // Perform lexical analysis on the raw input
    auto tokens = Token::tokenizeInput(raw_input, ctx);

    // Parse tokens and populate execution context
    Parser::parseTokens(tokens, ctx);
}

/**
 * @brief Loads an embedded JSON resource from the executable.
 *
 * This function retrieves a raw data resource (RT_RCDATA) that has been
 * embedded into the executable at build time (e.g. via resources.rc),
 * and returns its contents as a std::string.
 *
 * The resource is accessed directly from the module image in memory,
 * avoiding any filesystem dependency.
 *
 * @param resource_id Numeric resource identifier of the embedded JSON.
 * @return std::string Contents of the resource as a binary-safe string.
 *
 * @throws std::runtime_error If the resource cannot be found or loaded.
 */
std::string Shell::load_resource_json(int resource_id)
{
    // Obtain handle to the current executable module
    HMODULE hModule = GetModuleHandle(nullptr);

    // Locate the raw data resource by its numeric ID
    HRSRC hRes = FindResource(
        hModule,
        MAKEINTRESOURCE(resource_id),
        RT_RCDATA);
    if (!hRes)
    {
        throw std::runtime_error("FindResource failed");
    }

    // Load the resource into memory
    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData)
    {
        throw std::runtime_error("LoadResource failed");
    }

    // Determine the size of the resource in bytes
    DWORD size = SizeofResource(hModule, hRes);

    // Obtain a pointer to the resource's raw byte data
    const char *data =
        static_cast<const char *>(LockResource(hData));

    // Construct a binary-safe string from the resource data
    return std::string(data, size);
}
