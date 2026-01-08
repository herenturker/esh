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

// FILE: src\headers\Parser.hpp
// PURPOSE: Header file for 'src\core\Parser.cpp'. Parses tokens.

#pragma once

// INCLUDE LIBRARIES

#include <string>
#include <vector>

#include "Lexer.hpp"
#include "Commands.hpp"

class Parser
{
public:
    static void parseTokens(const std::vector<Lexer::Token> &tokens);

    static CommandType parseCommand(const std::wstring &token);

    static uint8_t parseFlags(const std::vector<std::wstring> &tokens);
};