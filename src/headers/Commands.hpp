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

// --------- DEFINE COMMAND CONSTANTS ---------------
// Command Types                         Hex Values  |
// --------------------------------------------------
#define COMMAND_RESERVED                    0X00
#define COMMAND_LS                          0X01
#define COMMAND_PWD                         0X02
#define COMMAND_EXIT                        0X03
#define COMMAND_CD                          0X04
#define COMMAND_WHOAMI                      0X05
#define COMMAND_DATETIME                    0X06
#define COMMAND_HOSTNAME                    0X07
#define COMMAND_DIR                         0X08
#define COMMAND_TOUCH                       0X09
#define COMMAND_RM                          0X0A
#define COMMAND_MKDIR                       0X0B
#define COMMAND_RMDIR                       0X0C
#define COMMAND_CLEAR                       0X0D // SAME AS CLS
#define COMMAND_MV                          0X0E
#define COMMAND_CP                          0X0F 
#define COMMAND_SYSTEMINFO                  0X10
#define COMMAND_SYSTEMSTATS                 0X11
#define COMMAND_REVIEW                      0X12
#define COMMAND_ECHO                        0X13
// -------------------------------------------------

#include <string>
#include <unordered_map>
#include <cstdint>

#include "Shell.hpp"

class Commands {
    public:
        static bool isBuiltInCommand(const std::wstring& command);    
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
    MV =                COMMAND_MV,
    CP =                COMMAND_CP,
    SYSTEMINFO =        COMMAND_SYSTEMINFO,
    SYSTEMSTATS =       COMMAND_SYSTEMSTATS,
    REVIEW =            COMMAND_REVIEW,
    ECHO =              COMMAND_ECHO
};

// DEFINE FLAGS
enum class Flag : uint8_t {
    RECURSIVE   = 0x01, // -r 
    VERBOSE     = 0x02, // -v 
    FORCE       = 0x04, // -f 
    ALL         = 0x08, // -a 
    HELP        = 0x10  // --help
};

// MAP COMMAND STRINGS TO COMMAND TYPES
static inline const std::unordered_map<std::wstring, CommandType> commandMap = {
    {L"ls", CommandType::LS},
    {L"pwd", CommandType::PWD},
    {L"exit", CommandType::EXIT},
    {L"cd", CommandType::CD},
    {L"whoami", CommandType::WHOAMI},
    {L"datetime", CommandType::DATETIME},
    {L"hostname", CommandType::HOSTNAME},
    {L"dir", CommandType::DIR},
    {L"touch", CommandType::TOUCH},
    {L"rm", CommandType::RM},
    {L"mkdir", CommandType::MKDIR},
    {L"rmdir", CommandType::RMDIR},
    {L"clear", CommandType::CLEAR},
    {L"cls", CommandType::CLEAR},
    {L"mv", CommandType::MV},
    {L"cp", CommandType::CP},
    {L"systeminfo", CommandType::SYSTEMINFO},
    {L"systemstats", CommandType::SYSTEMSTATS},
    {L"review", CommandType::REVIEW},
    {L"echo", CommandType::ECHO}
};

// MAP FLAG STRINGS TO FLAG TYPES
static inline const std::unordered_map<std::wstring, Flag> flagMap = {
    {L"-r", Flag::RECURSIVE},
    {L"-v", Flag::VERBOSE},
    {L"-f", Flag::FORCE},
    {L"-a", Flag::ALL},
    {L"--help", Flag::HELP}
};

