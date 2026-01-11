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

// FILE: src\headers\Token.hpp
// PURPOSE: Header file for ''. Created for handling circular dependency problem and performing operations related to tokens.

#pragma once

// INCLUDE LIBRARIES

#include <vector>
#include <string>

#include "Lexer.hpp"
#include "../execution/Execution.hpp"

class Token {

    public:
        /**
         * @brief Tokenizes the input string into a sequence of tokens.
         * @param input The raw input string.
         * @param ctx   Execution context (tracks pipeline/redirection state).
         * @return A vector of tokens.
         */
        static std::vector<Lexer::Token> tokenizeInput(const std::wstring &input, Execution::Executor::Context &ctx);

        /**
         * @brief Identifies the type of a given token string.
         * @param token The token string.
         * @param ctx   Execution context (updated if pipeline/redirection detected).
         * @return The token type.
         */
        static Lexer::TokenType identifyTokenType(const std::wstring &token, Execution::Executor::Context &ctx);

};

