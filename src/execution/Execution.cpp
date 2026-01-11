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

// FILE: src\execution\Execution.cpp
// PURPOSE: Performs redirections and pipelines.

// INCLUDE LIBRARIES

#include <windows.h>

#include "Execution.hpp"
#include "../headers/Parser.hpp"
#include "../headers/Engine.hpp"

// HELPER FUNCTIONS

static std::wstring buildCommandLine(const std::vector<Lexer::Token> &tokens)
{
    std::wstring result;

    for (const auto &t : tokens)
    {
        if (!result.empty())
            result += L" ";

        result += t.lexeme;
    }

    return result;
}

static std::vector<Lexer::Token>
stripRedirectionTokens(const std::vector<Lexer::Token> &tokens)
{
    std::vector<Lexer::Token> clean;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (Execution::Executor::getRedirectType(tokens[i]) != Execution::Executor::RedirectType::NONE)
        {
            ++i;
            continue;
        }
        clean.push_back(tokens[i]);
    }
    return clean;
}

// FUNCTIONS

void Execution::Executor::executeSimple(const std::vector<Lexer::Token> &tokens, Context& ctx)
{
    uint8_t command = 0;
    uint8_t flags = 0;
    std::vector<std::wstring> args;

    for (const auto &t : tokens)
    {
        if (t.type == Lexer::TOKEN_COMMAND)
            command = static_cast<uint8_t>(Parser::parseCommand(t.lexeme));
        else if (t.type == Lexer::TOKEN_FLAG)
            flags |= Parser::parseFlags({t.lexeme});
        else
            args.push_back(t.lexeme);
    }

    if (command != 0)
    { 
        Engine::execute(
            static_cast<CommandType>(command),
            flags,
            args, ctx);
    }
}

void Execution::Executor::run(const std::vector<Lexer::Token> &tokens, Context& ctx)
{
    const auto redir = hasRedirection(tokens);

    if (!hasPipeline(tokens) && !redir.hasRedirection)
    {
        ctx.pipelineEnabled = false;
        ctx.redirectionEnabled = false;

        executeSimple(tokens, ctx);
        return;
    } 
    else if (hasPipeline(tokens))
    {
        ctx.pipelineEnabled = true;
        executePipeline(tokens);
        
    } 
    else if (redir.hasRedirection)
    {
        ctx.redirectionEnabled = true;

        auto redirInfo = parseRedirections(tokens);
        auto cleanTokens = stripRedirectionTokens(tokens);

        HANDLE oldIn  = ctx.stdinHandle;
        HANDLE oldOut = ctx.stdoutHandle;
        HANDLE oldErr = ctx.stderrHandle;

        if (redirInfo.stdinHandle)
            ctx.stdinHandle = redirInfo.stdinHandle;
        if (redirInfo.stdoutHandle)
            ctx.stdoutHandle = redirInfo.stdoutHandle;
        if (redirInfo.stderrHandle)
            ctx.stderrHandle = redirInfo.stderrHandle;

        executeSimple(cleanTokens, ctx);

        ctx.stdinHandle  = oldIn;
        ctx.stdoutHandle = oldOut;
        ctx.stderrHandle = oldErr;

        if (redirInfo.stdinHandle)  CloseHandle(redirInfo.stdinHandle);

        if (redirInfo.stdoutHandle && redirInfo.stdoutHandle != redirInfo.stderrHandle)
            CloseHandle(redirInfo.stdoutHandle);
            
        if (redirInfo.stderrHandle)
            CloseHandle(redirInfo.stderrHandle);
    }

}

bool Execution::Executor::hasPipeline(const std::vector<Lexer::Token> &tokens)
{

    for (const auto &t : tokens)
    {
        if (t.type == Lexer::TOKEN_PIPELINE && t.lexeme == L"|") // double check
        {
            return true;
        }
    }

    return false;
}

Execution::Executor::Redirection Execution::Executor::hasRedirection(const std::vector<Lexer::Token> &tokens)
{
    for (const auto &t : tokens)
    {
        if (t.type == Lexer::TOKEN_OUTPUT_REDIRECTION_ONE ||
            t.type == Lexer::TOKEN_OUTPUT_REDIRECTION_TWO ||
            t.type == Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_ONE ||
            t.type == Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_TWO ||
            t.type == Lexer::TOKEN_ERROR_REDIRECTION_ONE ||
            t.type == Lexer::TOKEN_ERROR_REDIRECTION_TWO)
        {
            return {true, t.lexeme};
        }
    }

    return {false, L""};
}

std::vector<std::vector<Lexer::Token>>
Execution::Executor::splitByPipeline(const std::vector<Lexer::Token> &tokens)
{
    std::vector<std::vector<Lexer::Token>> result;
    result.emplace_back();

    for (const auto &t : tokens)
    {
        if (t.type == Lexer::TOKEN_PIPELINE)
        {
            result.emplace_back();
        }
        else
        {
            result.back().push_back(t);
        }
    }

    return result;
}

void Execution::Executor::executePipeline(const std::vector<Lexer::Token> &tokens)
{
    auto commands = splitByPipeline(tokens);

    HANDLE prevRead = NULL;

    for (size_t i = 0; i < commands.size(); ++i)
    {
        HANDLE readPipe = NULL;
        HANDLE writePipe = NULL;

        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = nullptr;
        sa.bInheritHandle = TRUE;

        if (i < commands.size() - 1)
        {
            CreatePipe(&readPipe, &writePipe, &sa, 0);
        }

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;

        si.hStdInput = prevRead ? prevRead : GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = (i < commands.size() - 1)
                            ? writePipe
                            : GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

        auto redir = parseRedirections(commands[i]);

        // STDIN
        if (i == 0 && redir.stdinHandle)
        {
            si.hStdInput = redir.stdinHandle;
        }
        else if (prevRead)
        {
            si.hStdInput = prevRead;
        }

        // STDOUT
        if (i < commands.size() - 1)
        {
            si.hStdOutput = writePipe;
        }
        else
        {
            si.hStdOutput = redir.stdoutHandle
                                ? redir.stdoutHandle
                                : GetStdHandle(STD_OUTPUT_HANDLE);
        }

        // STDERR
        si.hStdError = redir.stderrHandle
                           ? redir.stderrHandle
                           : GetStdHandle(STD_ERROR_HANDLE);

        PROCESS_INFORMATION pi{};

        auto cleanTokens = stripRedirectionTokens(commands[i]);
        std::wstring cmdLine = buildCommandLine(cleanTokens);

        std::vector<wchar_t> buffer(cmdLine.begin(), cmdLine.end());
        buffer.push_back(L'\0');

        CreateProcessW(
            nullptr,
            buffer.data(),
            nullptr,
            nullptr,
            TRUE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi);

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        if (prevRead)
            CloseHandle(prevRead);

        if (writePipe)
            CloseHandle(writePipe);

        prevRead = readPipe;

        if (redir.stdinHandle)
            CloseHandle(redir.stdinHandle);

        if (redir.stdoutHandle && redir.stdoutHandle != redir.stderrHandle)
            CloseHandle(redir.stdoutHandle);

        if (redir.stderrHandle)
            CloseHandle(redir.stderrHandle);
    }
}

Execution::Executor::RedirectType Execution::Executor::getRedirectType(const Lexer::Token &t)
{
    switch (t.type)
    {
    case Lexer::TOKEN_INPUT_REDIRECTION:
        return Execution::Executor::RedirectType::STDIN;

    case Lexer::TOKEN_OUTPUT_REDIRECTION_ONE: // >
    case Lexer::TOKEN_OUTPUT_REDIRECTION_TWO: // >>
        return Execution::Executor::RedirectType::STDOUT;

    case Lexer::TOKEN_ERROR_REDIRECTION_ONE: // 2>
    case Lexer::TOKEN_ERROR_REDIRECTION_TWO: // 2>>
        return Execution::Executor::RedirectType::STDERR;

    case Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_ONE: // &>
    case Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_TWO: // &>>
        return Execution::Executor::RedirectType::STDOUT_STDERR;

    default:
        return Execution::Executor::RedirectType::NONE;
    }
}

bool Execution::Executor::isAppendRedirection(const Lexer::Token &t)
{
    return t.type == Lexer::TOKEN_OUTPUT_REDIRECTION_TWO ||
           t.type == Lexer::TOKEN_ERROR_REDIRECTION_TWO ||
           t.type == Lexer::TOKEN_OUTPUT_ERROR_REDIRECTION_TWO;
}

HANDLE Execution::Executor::openFileForWrite(const std::wstring &path, bool append)
{
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    //                          append         write
    DWORD creation = append ? OPEN_ALWAYS : CREATE_ALWAYS;
    DWORD access = append ? FILE_APPEND_DATA : GENERIC_WRITE;

    HANDLE h = CreateFileW(
        path.c_str(),
        access,
        FILE_SHARE_READ,
        &sa,
        creation,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    return (h == INVALID_HANDLE_VALUE) ? nullptr : h;
}

HANDLE Execution::Executor::openFileForRead(const std::wstring &path)
{
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFileW(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    return (h == INVALID_HANDLE_VALUE) ? nullptr : h;
}

Execution::Executor::RedirectionInfo Execution::Executor::parseRedirections(const std::vector<Lexer::Token> &tokens)
{
    Execution::Executor::RedirectionInfo info;

    for (size_t i = 0; i + 1 < tokens.size(); ++i)
    {
        Execution::Executor::RedirectType type = getRedirectType(tokens[i]);
        if (type == Execution::Executor::RedirectType::NONE)
            continue;

        const std::wstring &target = tokens[i + 1].lexeme;
        bool append = Execution::Executor::isAppendRedirection(tokens[i]);

        switch (type)
        {
        case Execution::Executor::RedirectType::STDIN:
            if (info.stdinHandle)
                CloseHandle(info.stdinHandle);
            info.stdinHandle = Execution::Executor::openFileForRead(target);
            break;

        case Execution::Executor::RedirectType::STDOUT:
            if (info.stdoutHandle)
                CloseHandle(info.stdoutHandle);
            info.stdoutHandle = Execution::Executor::openFileForWrite(target, append);
            break;

        case Execution::Executor::RedirectType::STDERR:
            if (info.stderrHandle)
                CloseHandle(info.stderrHandle);
            info.stderrHandle = Execution::Executor::openFileForWrite(target, append);
            break;

        case Execution::Executor::RedirectType::STDOUT_STDERR:
        {
            HANDLE h = Execution::Executor::openFileForWrite(target, append);
            if (info.stdoutHandle)
                CloseHandle(info.stdoutHandle);
            if (info.stderrHandle)
                CloseHandle(info.stderrHandle);
            info.stdoutHandle = h;
            info.stderrHandle = h;
            break;
        }

        default:
            break;
        }
    }

    return info;
}