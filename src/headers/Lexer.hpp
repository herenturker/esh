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

// FILE: src\headers\Lexer.hpp
// PURPOSE: Header file for 'src\core\Lexer.cpp'. Performs lexical analysis.

#pragma once

// INCLUDE LIBRARIES
#include <string>
#include <vector>

// #include "../execution/Execution.hpp"

class Lexer
{
public:


    /**
     * @brief Represents different types of tokens that can be identified during lexical analysis.
     */
    enum TokenType
    {
        TOKEN_COMMAND,   /** Token representing a command (e.g., ls, cd) */
        TOKEN_FLAG,      /** Token representing a flag (e.g., -f) */
        TOKEN_EXECUTEE,  /** Token representing an executee (e.g., a script or program to run) */

        TOKEN_NUMBER,    /** Numeric literal (decimal, hex, etc.) */
        TOKEN_STRING,    /** String literal (e.g., "Hello") */

        // === Symbols & punctuation ===
        TOKEN_COMMA,     /** , */
        TOKEN_COLON,     /** : */
        TOKEN_DOT,       /** . */
        TOKEN_PLUS,      /** + */
        TOKEN_MINUS,     /** - */
        TOKEN_STAR,      /** * */
        TOKEN_MODULO,    /** % (modulo symbol) */
        TOKEN_SEMICOLON, /** ; */

        TOKEN_OPEN_PARENTHESIS,   /** ( */
        TOKEN_CLOSE_PARENTHESIS,  /** ) */
        TOKEN_OPEN_BRACKET,       /** [ */
        TOKEN_CLOSE_BRACKET,      /** ] */

        TOKEN_DOLLAR_SIGN,        /** $ */
        TOKEN_HASH_SIGN,          /** # */
        TOKEN_AT_SIGN,            /** @ */
        TOKEN_CARET,              /** ^ */
        TOKEN_AMPERSAND,          /** & */
        TOKEN_TILDE,              /** ~ */
        TOKEN_EQUAL,              /** = */

        TOKEN_OUTPUT_REDIRECTION_ONE,        /** >   */
        TOKEN_OUTPUT_REDIRECTION_TWO,        /** >>  */
        TOKEN_INPUT_REDIRECTION,             /** <   */

        TOKEN_ERROR_REDIRECTION_ONE,         /** 2>  */
        TOKEN_ERROR_REDIRECTION_TWO,         /** 2>> */

        TOKEN_OUTPUT_ERROR_REDIRECTION_ONE,  /** &>  */
        TOKEN_OUTPUT_ERROR_REDIRECTION_TWO,  /** &>> */

        TOKEN_PIPELINE,                      /** |   */

        TOKEN_EOF                            /** End of input */
    };

    /**
     * @brief Represents a single token with its type and lexeme.
     */
    struct Token
    {
        TokenType type;
        std::wstring lexeme;
    };

    /**
     * @brief Tokenizes the input string into a sequence of tokens.
     * @param input The raw input string.
     * @param ctx   Execution context (tracks pipeline/redirection state).
     * @return A vector of tokens.
     */
    static std::vector<Token> tokenizeInput(const std::wstring &input, Execution::Executor::Context& ctx);

    /**
     * @brief Identifies the type of a given token string.
     * @param token The token string.
     * @param ctx   Execution context (updated if pipeline/redirection detected).
     * @return The token type.
     */
    static TokenType identifyTokenType(const std::wstring &token, Execution::Executor::Context& ctx);
};
