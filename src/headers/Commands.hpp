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

// FILE: src\headers\Commands.hpp
// PURPOSE: Header file for 'src\core\Commands.cpp'. Commands' and flags' definitons are made here.
// DO NOT FORGET to check 'esh.json'.

#pragma once

// --------- DEFINE COMMAND CONSTANTS ----------------
// Command Types                         Hex Values  |
// ---------------------------------------------------
#define COMMAND_RESERVED                    0x00
#define COMMAND_LS                          0x01
#define COMMAND_PWD                         0x02
#define COMMAND_EXIT                        0x03
#define COMMAND_CD                          0x04
#define COMMAND_WHOAMI                      0x05
#define COMMAND_DATETIME                    0x06
#define COMMAND_HOSTNAME                    0x07
#define COMMAND_PS                          0x08
#define COMMAND_TOUCH                       0x09
#define COMMAND_RM                          0x0A
#define COMMAND_MKDIR                       0x0B
#define COMMAND_RMDIR                       0x0C
#define COMMAND_CLEAR                       0x0D
#define COMMAND_MV                          0x0E
#define COMMAND_CP                          0x0F
#define COMMAND_SYSTEMINFO                  0x10
#define COMMAND_SYSTEMSTATS                 0x11
#define COMMAND_REW                         0x12
#define COMMAND_ECHO                        0x13
#define COMMAND_STATS                       0x14
#define COMMAND_HEAD                        0x15
#define COMMAND_TAIL                        0x16
#define COMMAND_KILL                        0x17

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-



// ------------- DEFINE FLAG CONSTANTS ---------------
// Flag Types                            Hex Values  |
// ---------------------------------------------------
#define FLAG_RECURSIVE                      0x01   // -r                           00000001
#define FLAG_VERBOSE                        0x02   // -v                           00000010
#define FLAG_FORCE                          0x04   // -f                           00000100
#define FLAG_ALL                            0x08   // -a                           00001000
#define FLAG_HELP                           0x10   // --help                       00010000
#define FLAG_COUNT                          0x20   // -n (used for line counts)    00100000

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-



// ------------------ DEFINE SYMBOLS  ----------------
// Symbol Types                           Hex Values |
// ---------------------------------------------------
#define SYMBOL_OUTPUT_REDIRECTION_ONE        0x01 // >
#define SYMBOL_OUTPUT_REDIRECTION_TWO        0x02 // >>
#define SYMBOL_INPUT_REDIRECTION             0x03 // <

#define SYMBOL_ERROR_REDIRECTION_ONE         0x04 // 2>
#define SYMBOL_ERROR_REDIRECTION_TWO         0x05 // 2>>

#define SYMBOL_OUTPUT_ERROR_REDIRECTION_ONE  0x06 // &>
#define SYMBOL_OUTPUT_ERROR_REDIRECTION_TWO  0x07 // &>>

#define SYMBOL_PIPELINE                      0x08 // |

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-


// INCLUDE LIBRARIES

#include <string>
#include <unordered_map>
#include <cstdint>

#include "Shell.hpp"

class Commands
{
public:
    static bool isBuiltInCommand(const std::wstring &command);
};

// DEFINE COMMAND TYPES
enum class CommandType : uint8_t
{
    RESERVED =       COMMAND_RESERVED,
    LS =             COMMAND_LS,
    PWD =            COMMAND_PWD,
    EXIT =           COMMAND_EXIT,
    CD =             COMMAND_CD,
    WHOAMI =         COMMAND_WHOAMI,
    DATETIME =       COMMAND_DATETIME,
    HOSTNAME =       COMMAND_HOSTNAME,
    TOUCH =          COMMAND_TOUCH,
    RM =             COMMAND_RM,
    MKDIR =          COMMAND_MKDIR,
    RMDIR =          COMMAND_RMDIR,
    CLEAR =          COMMAND_CLEAR,
    MV =             COMMAND_MV,
    CP =             COMMAND_CP,
    SYSTEMINFO =     COMMAND_SYSTEMINFO,
    SYSTEMSTATS =    COMMAND_SYSTEMSTATS,
    REW =            COMMAND_REW,
    ECHO =           COMMAND_ECHO,
    STATS =          COMMAND_STATS,
    HEAD =           COMMAND_HEAD,
    TAIL =           COMMAND_TAIL,
    PS =             COMMAND_PS,
    KILL =           COMMAND_KILL
};

// DEFINE FLAGS
enum class Flag : uint8_t
{
    RECURSIVE =     FLAG_RECURSIVE, // -r
    VERBOSE =       FLAG_VERBOSE,     // -v
    FORCE =         FLAG_FORCE,         // -f
    ALL =           FLAG_ALL,             // -a
    HELP =          FLAG_HELP,           // --help
    COUNT =         FLAG_COUNT          // -n (used for line counts)
};

// DEFINE SYMBOLS

enum class Symbol : uint8_t
{
    OUTPUT_REDIRECTION_ONE =            SYMBOL_OUTPUT_REDIRECTION_ONE,
    OUTPUT_REDIRECTION_TWO =            SYMBOL_OUTPUT_REDIRECTION_TWO,
    INPUT_REDIRECTION =                 SYMBOL_INPUT_REDIRECTION,
    ERROR_REDIRECTION_ONE =             SYMBOL_ERROR_REDIRECTION_ONE,
    ERROR_REDIRECTION_TWO =             SYMBOL_ERROR_REDIRECTION_TWO,
    OUTPUT_ERROR_REDIRECTION_ONE =      SYMBOL_OUTPUT_ERROR_REDIRECTION_ONE,
    OUTPUT_ERROR_REDIRECTION_TWO =      SYMBOL_OUTPUT_ERROR_REDIRECTION_TWO,
    PIPELINE =                          SYMBOL_PIPELINE

};
// COMMAND GROUP FOR ENGINE
enum class CommandGroup : uint8_t
{
    FILE_IO,
    PROCESS,
    ENVIRONMENT,
    SHELL,
    SYSTEM,
    UNKNOWN
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
    {L"touch", CommandType::TOUCH},
    {L"rm", CommandType::RM},
    {L"mkdir", CommandType::MKDIR},
    {L"rmdir", CommandType::RMDIR},
    {L"clear", CommandType::CLEAR},
    {L"mv", CommandType::MV},
    {L"cp", CommandType::CP},
    {L"systeminfo", CommandType::SYSTEMINFO},
    {L"systemstats", CommandType::SYSTEMSTATS},
    {L"rew", CommandType::REW},
    {L"echo", CommandType::ECHO},
    {L"stats", CommandType::STATS},
    {L"head", CommandType::HEAD},
    {L"tail", CommandType::TAIL},
    {L"ps", CommandType::PS},
    {L"kill", CommandType::KILL}
};

// MAP FLAG STRINGS TO FLAG TYPES
static inline const std::unordered_map<std::wstring, Flag> flagMap = {
    {L"-r", Flag::RECURSIVE},
    {L"-v", Flag::VERBOSE},
    {L"-f", Flag::FORCE},
    {L"-a", Flag::ALL},
    {L"--help", Flag::HELP},
    {L"-n", Flag::COUNT}
};

// MAP SYMBOL STRINGS TO SYMBOL TYPES
static inline const std::unordered_map<std::wstring, Symbol> symbolMap = {
    {L">", Symbol::OUTPUT_REDIRECTION_ONE},
    {L">>", Symbol::OUTPUT_REDIRECTION_TWO},
    {L"<", Symbol::INPUT_REDIRECTION},
    {L"2>", Symbol::ERROR_REDIRECTION_ONE},
    {L"2>>", Symbol::ERROR_REDIRECTION_TWO},
    {L"&>", Symbol::OUTPUT_ERROR_REDIRECTION_ONE},
    {L"&>>", Symbol::OUTPUT_ERROR_REDIRECTION_TWO},
    {L"|", Symbol::PIPELINE}
};

// Get command group from command given.
inline CommandGroup getCommandGroup(CommandType cmd)
{
    switch (cmd)
    {
    // -------- FILE COMMANDS --------
    case CommandType::LS:
    case CommandType::STATS:
    case CommandType::HEAD:
    case CommandType::TAIL:
    case CommandType::TOUCH:
    case CommandType::RM:
    case CommandType::CP:
    case CommandType::MV:
    case CommandType::MKDIR:
    case CommandType::RMDIR:
    case CommandType::REW:
        return CommandGroup::FILE_IO;

    // -------- PROCESS COMMANDS --------
    case CommandType::PS:
    case CommandType::KILL:
        return CommandGroup::PROCESS;

    // -------- ENVIRONMENT COMMANDS --------
    case CommandType::CD:
    case CommandType::PWD:
    case CommandType::WHOAMI:
    case CommandType::DATETIME:
    case CommandType::HOSTNAME:
        return CommandGroup::ENVIRONMENT;

    // -------- SHELL COMMANDS --------
    case CommandType::EXIT:
    case CommandType::CLEAR:
    case CommandType::ECHO:
        return CommandGroup::SHELL;

    // -------- SYSTEM COMMANDS --------
    case CommandType::SYSTEMINFO:
    case CommandType::SYSTEMSTATS:
        return CommandGroup::SYSTEM;

    default:
        return CommandGroup::UNKNOWN;
    }
}
