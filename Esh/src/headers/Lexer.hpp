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

#pragma once
#include <string>
#include <vector>

class Lexer {

    public:

        /**
         * @brief Represents different types of tokens that can be identified during lexical analysis.
         */
        typedef enum {
            
            TOKEN_COMMAND,          /** Token representing a command (e.g., ls, cd) */
            TOKEN_FLAG,             /** Token representing a flag (e.g., -f) */
            TOKEN_EXECUTEE,        /** Token representing an executee (e.g., a script or program to run) */

            TOKEN_NUMBER,           /** Numeric literal (decimal, hex, etc.) */
            TOKEN_STRING,           /** String literal (e.g., "Hello") */

            // === Symbols & punctuation ===
            TOKEN_COMMA,            /** , */
            TOKEN_COLON,            /** : */
            TOKEN_DOT,              /** . */
            TOKEN_PLUS,             /** + */
            TOKEN_MINUS,            /** - */
            TOKEN_STAR,              /** * */
            TOKEN_MODULO,           /** % (modulo symbol) */
            TOKEN_SEMICOLON,        /** ; */

            TOKEN_OPEN_PARENTHESIS,     /** ( */
            TOKEN_CLOSE_PARENTHESIS,    /** ) */
            TOKEN_OPEN_BRACKET,         /** [ */
            TOKEN_CLOSE_BRACKET,        /** ] */

            TOKEN_DOLLAR_SIGN,      /** $ */
            TOKEN_HASH_SIGN,        /** # */
            TOKEN_AT_SIGN,          /** @ */
            TOKEN_CARET,            /** ^ */
            TOKEN_AMPERSAND,        /** & */
            TOKEN_PIPE,             /** | */
            TOKEN_TILDE,            /** ~ */
            TOKEN_EQUAL,            /** = */
            TOKEN_LESS_THAN,        /** < */
            TOKEN_GREATER_THAN,     /** > */
            TOKEN_EOF               /** End of input */

        } TokenType;

        struct Token {
            TokenType type;
            std::string lexeme;
        };
        
        static std::vector<Token> tokenizeInput(const std::string& input);

        static TokenType identifyTokenType(const std::string& token);
};