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

#include <iostream>
#include <string>

#include "headers/Lexer.h"

void Lexer::tokenizeInput(std::string& raw_input) {
    
    // Check for empty input
    if (raw_input.empty()) {
        return;
    }

    while (true) {

        // Find the position of the first space
        size_t space_pos = raw_input.find(' ');

        // If no space is found, the rest of the string is a token
        if (space_pos == std::string::npos) {
            std::string token = raw_input;

            if (!token.empty()) {
                std::cout << "Token: " << token << std::endl;
            }
            break;
        }

        // Extract the token
        std::string token = raw_input.substr(0, space_pos);
        if (!token.empty()) {
            std::cout << "Token: " << token << std::endl;
        }

        // Remove the processed token and leading spaces from the input
        raw_input = raw_input.substr(space_pos + 1);
        while (!raw_input.empty() && raw_input[0] == ' ') {
            raw_input.erase(0, 1);
        }
        
        // If the remaining input is empty, break the loop
        if (raw_input.empty()) {
            break;
        }
    }

}