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

#include "headers/Shell.h"
#include "headers/Lexer.h"

std::string Shell::raw_input_gathered;

void Shell::handleRawInput(std::string& raw_input) {
    raw_input_gathered = raw_input;
    Lexer::tokenizeInput(raw_input_gathered);
}
