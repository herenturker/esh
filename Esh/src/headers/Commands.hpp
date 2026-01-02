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

// --------- DEFINE COMMAND CONSTANTS ---------

#define COMMAND_RESERVED 0X00
#define COMMAND_LS 0X01
#define COMMAND_PWD 0X02
#define COMMAND_EXIT 0X03
#define COMMAND_CD 0X04
#define COMMAND_WHOAMI 0X05
#define COMMAND_DATETIME 0X06
#define COMMAND_HOSTNAME 0X07
#define COMMAND_DIR 0X08
#define COMMAND_TOUCH 0X09
#define COMMAND_RM 0X0A
#define COMMAND_MKDIR 0X0B
#define COMMAND_RMDIR 0X0C
#define COMMAND_CLEAR 0X0D // SAME AS CLS
// --------------------------------------------

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
    RESERVED =          COMMAND_RESERVED,
    LS  =               COMMAND_LS,
    PWD =               COMMAND_PWD,
    EXIT =              COMMAND_EXIT,
    CD =                COMMAND_CD,
    WHOAMI =            COMMAND_WHOAMI,
    DATETIME =          COMMAND_DATETIME,
    HOSTNAME =          COMMAND_HOSTNAME,
    DIR =               COMMAND_DIR,
    TOUCH =             COMMAND_TOUCH,
    RM =                COMMAND_RM,
    MKDIR =             COMMAND_MKDIR,
    RMDIR =             COMMAND_RMDIR,
    CLEAR =             COMMAND_CLEAR,
    CLS =               COMMAND_CLEAR, // SAME AS CLEAR
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
    {"touch", CommandType::TOUCH},
    {"rm", CommandType::RM},
    {"mkdir", CommandType::MKDIR},
    {"rmdir", CommandType::RMDIR},
    {"clear", CommandType::CLEAR},
    {"cls", CommandType::CLEAR}
};

// MAP FLAG STRINGS TO FLAG TYPES
static inline const std::unordered_map<std::string, Flag> flagMap = {
    {"-r", Flag::RECURSIVE},
    {"-v", Flag::VERBOSE},
    {"-f", Flag::FORCE},
    {"-a", Flag::ALL}
};

