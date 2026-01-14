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

// FILE: src\core\Parser.cpp
// PURPOSE: Parses the tokens given from 'Lexer.cpp'.

// INCLUDE LIBRARIES

#include <stdint.h>
#include <string>

#include "../headers/Parser.hpp"
#include "../execution/Execution.hpp"

/**
 * @brief Parses a sequence of lexer tokens and dispatches execution.
 *
 * Acts as the bridge between parsing and execution layers. This function
 * forwards the fully tokenized input to the executor, which is responsible
 * for handling pipelines, redirections, and command execution.
 *
 * @param tokens The list of tokens produced by the lexer.
 * @param ctx Execution context carrying runtime state and I/O handles.
 */
void Parser::parseTokens(const std::vector<Lexer::Token>& tokens, Execution::Executor::Context& ctx)
{
    Execution::Executor::run(tokens, ctx);
}

/**
 * @brief Resolves a command token to its corresponding command type.
 *
 * Looks up the given command string in the internal command map and returns
 * the associated CommandType enum value.
 *
 * If the command is not found, a reserved command value is returned.
 *
 * @param token The command token as a wide string.
 * @return The resolved CommandType, or a reserved value if not found.
 */
CommandType Parser::parseCommand(const std::wstring &token)
{
    auto it = commandMap.find(token);
    if (it != commandMap.end())
        return it->second;
    return static_cast<CommandType>(0x00); // RESERVED
}

/**
 * @brief Parses command-line flags into a bitmask.
 *
 * Iterates over the provided flag tokens and accumulates their corresponding
 * flag values into a single bitmask using bitwise OR operations.
 *
 * Unknown flags are silently ignored.
 *
 * @param tokens A list of flag tokens.
 * @return An 16-bit bitmask representing the parsed flags.
 */
uint16_t Parser::parseFlags(const std::vector<std::wstring> &tokens)
{
    uint16_t result = 0;
    for (const auto &t : tokens)
    {
        auto it = flagMap.find(t);
        if (it != flagMap.end())
        {
            result |= static_cast<uint16_t>(it->second);
        }
    }
    return result;
}
