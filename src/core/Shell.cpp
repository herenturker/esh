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

#include <windows.h>
#include <string>
#include <stdexcept>
#include <iostream>

#include "../headers/Shell.hpp"
#include "../headers/Lexer.hpp"
#include "../headers/Parser.hpp"

void Shell::handleRawInput(std::wstring& raw_input) {

    auto tokens = Lexer::tokenizeInput(raw_input);
    Parser::parseTokens(tokens);
    
}

std::string Shell::load_resource_json(int resource_id) {
    HMODULE hModule = GetModuleHandle(nullptr);

    HRSRC hRes = FindResource(hModule,
                             MAKEINTRESOURCE(resource_id),
                             RT_RCDATA);
    if (!hRes) {
        throw std::runtime_error("FindResource failed");
    }

    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData) {
        throw std::runtime_error("LoadResource failed");
    }

    DWORD size = SizeofResource(hModule, hRes);
    const char* data = static_cast<const char*>(LockResource(hData));

    return std::string(data, size);
}
