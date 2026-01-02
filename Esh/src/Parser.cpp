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

#include <stdint.h>
#include <string>

#include "headers/Parser.hpp"
#include "headers/Engine.hpp"

void Parser::parseTokens(const std::vector<Lexer::Token> &tokens)
{

    uint8_t command = 0;
    uint8_t flagAccumulator = 0;
    std::string executee;

    for (const auto &t : tokens)
    {

        if (t.type == Lexer::TOKEN_COMMAND)
        {
            auto commandType = Parser::parseCommand(t.lexeme);
            command = static_cast<uint8_t>(commandType);
        }

        if (t.type == Lexer::TOKEN_FLAG)
        {
            auto flagValue = Parser::parseFlags({t.lexeme});
            flagAccumulator |= flagValue;
        }

        if (t.type == Lexer::TOKEN_EXECUTEE)
        {
            executee = t.lexeme;
        }
    }

    if (command != 0) {
        Engine::execute(static_cast<CommandType>(command), flagAccumulator, executee);
    }

}

CommandType Parser::parseCommand(const std::string &token)
{
    auto it = commandMap.find(token);
    if (it != commandMap.end())
        return it->second;
    return static_cast<CommandType>(0x00); // RESERVED
}

uint8_t Parser::parseFlags(const std::vector<std::string> &tokens)
{
    uint8_t result = 0;
    for (const auto &t : tokens)
    {
        auto it = flagMap.find(t);
        if (it != flagMap.end())
        {
            result |= static_cast<uint8_t>(it->second);
        }
    }
    return result;
}
