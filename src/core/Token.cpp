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

// FILE: src\core\Token.cpp
// PURPOSE: Performs operations related to tokens. Created for handling circular dependency problem.

// INCLUDE LIBRARIES

#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

#include "../headers/Token.hpp"
#include "../headers/Commands.hpp"

std::vector<Lexer::Token> Token::tokenizeInput(const std::wstring &input, Execution::Executor::Context& ctx)
{
    std::vector<Lexer::Token> tokens;

    std::size_t pos = 0;
    const std::size_t len = input.length();

    while (pos < len)
    {

        while (pos < len && input[pos] == ' ')
        {
            ++pos;
        }

        if (pos >= len)
            break;

        std::size_t start = pos;
        while (pos < len && input[pos] != ' ')
        {
            ++pos;
        }

        std::wstring token = input.substr(start, pos - start); // Gets the token without blank char.
        Lexer::TokenType type = Token::identifyTokenType(token, ctx);

        tokens.push_back({type, token});
    }

    tokens.push_back({Lexer::TOKEN_EOF, L""}); // End of input

    return tokens;
}

Lexer::TokenType Token::identifyTokenType(const std::wstring &token, Execution::Executor::Context& ctx)
{

    // Numeric (supports negative integers)
    if (token[0] == L'-' && token.size() > 1)
    {
        if (std::all_of(token.begin() + 1, token.end(),
                        [](unsigned char c)
                        { return std::isdigit(c); }))
        {
            return Lexer::TOKEN_NUMBER;
        }
        return Lexer::TOKEN_FLAG;
    }

    if (std::all_of(token.begin(), token.end(),
                    [](unsigned char c)
                    { return std::isdigit(c); }))
    {
        return Lexer::TOKEN_NUMBER;
    }

    if (token.size() >= 2 &&
        token.front() == L'"' &&
        token.back()  == L'"')
    {
        return Lexer::TOKEN_STRING;
    }

    if (token == L"|")
    {
        ctx.pipelineEnabled = true;
        return Lexer::TOKEN_PIPELINE;
    }
        

    if (token == L"<")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_INPUT_REDIRECTION;
    }
        

    if (token == L">>")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_OUTPUT_REDIRECTION_TWO;
    }
        

    if (token == L">")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_OUTPUT_REDIRECTION_ONE;
    }
        

    if (token == L"2>>")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_ERROR_REDIRECTION_TWO;
    }
        

    if (token == L"2>")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_ERROR_REDIRECTION_ONE;
    }
        

    if (token == L"&>>")
    {
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_TWO;
    }
        

    if (token == L"&>")
    {   
        ctx.redirectionEnabled = true;
        return Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_ONE;
    }
        
    if (Commands::isBuiltInCommand(token))
    {
        return Lexer::TOKEN_COMMAND;
    }

    return Lexer::TOKEN_EXECUTEE;
}
