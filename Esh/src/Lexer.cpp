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

#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

#include "headers/Lexer.hpp"
#include "headers/Commands.hpp"

std::vector<Lexer::Token> Lexer::tokenizeInput(const std::string& input) {

    std::vector<Token> tokens;

    std::size_t pos = 0;
    const std::size_t len = input.length();

    while (pos < len) {

        while (pos < len && input[pos] == ' ') {
            ++pos;
        }
        if (pos >= len) break;

        std::size_t start = pos;
        while (pos < len && input[pos] != ' ') {
            ++pos;
        }

        std::string token = input.substr(start, pos - start);
        TokenType type = identifyTokenType(token);

        tokens.push_back({ type, token });
    }

    tokens.push_back({ TOKEN_EOF, "" });

    return tokens;
}

Lexer::TokenType Lexer::identifyTokenType(const std::string& token) {

    // Numeric (supports negative integers)
    if (token[0] == '-' && token.size() > 1) {
        if (std::all_of(token.begin() + 1, token.end(),
                        [](unsigned char c) { return std::isdigit(c); })) {
            return TOKEN_NUMBER;
        }
        return TOKEN_FLAG;
    }

    if (std::all_of(token.begin(), token.end(),
                    [](unsigned char c) { return std::isdigit(c); })) {
        return TOKEN_NUMBER;
    }

    if (Commands::isBuiltInCommand(token)) {
        return TOKEN_COMMAND;
    }

    return TOKEN_EXECUTEE;
}
