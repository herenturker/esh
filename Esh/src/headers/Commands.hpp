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
#include <unordered_map>
#include <cstdint>

#include "Shell.hpp"

class Commands {

    public:
        static bool isBuiltInCommand(const std::string& command);
        
};
// DEFINE COMMAND TYPES
enum class CommandType : uint8_t {
    UNKNOWN = 0x00,
    LS  = 0x01,
    PWD = 0x02,
    EXIT = 0x03,
    CD = 0x04,
    WHOAMI = 0x05,
    DATETIME = 0x06,
    HOSTNAME = 0x07,
    DIR = 0x08,
    TOUCH = 0x09
};

// DEFINE FLAGS
enum class Flag : uint8_t {
    RECURSIVE   = 0x01, // -r 
    VERBOSE     = 0x02, // -v 
    FORCE       = 0x04, // -f 
    ALL         = 0x08  // -a 
};

// MAP COMMAND STRINGS TO COMMAND TYPES
static inline const std::unordered_map<std::string, CommandType> commandMap = {
    {"ls", CommandType::LS},
    {"pwd", CommandType::PWD},
    {"exit", CommandType::EXIT},
    {"cd", CommandType::CD},
    {"whoami", CommandType::WHOAMI},
    {"datetime", CommandType::DATETIME},
    {"hostname", CommandType::HOSTNAME},
    {"dir", CommandType::DIR},
    {"touch", CommandType::TOUCH}

};

// MAP FLAG STRINGS TO FLAG TYPES
static inline const std::unordered_map<std::string, Flag> flagMap = {
    {"-r", Flag::RECURSIVE},
    {"-v", Flag::VERBOSE},
    {"-f", Flag::FORCE},
    {"-a", Flag::ALL}
};

