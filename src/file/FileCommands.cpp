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

// FILE: src\file\FileCommands.cpp
// PURPOSE: Executes commands related to file operations and IO.

// INCLUDE LIBRARIES

#include <vector>
#include <string>
#include <iostream>
#include <deque>

#include <windows.h>

#include "../headers/Engine.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "../headers/Unicode.hpp"
#include "../headers/Helper.hpp"
#include "../execution/Execution.hpp"
#include "FileCommands.hpp"

// HELPER FUNCTIONS

/// @brief Writes a string to the given handle, considering console vs. file output.
/// @param h The Windows HANDLE (stdout, stderr, or file handle).
/// @param text The text to write.
static void writeOut(HANDLE h, const std::wstring &text)
{
    DWORD mode;
    if (GetConsoleMode(h, &mode))
    {
        WriteConsoleW(h, text.c_str(), static_cast<DWORD>(text.size()), nullptr, nullptr);
        console::writeln(text);
    }
    else
    {
        DWORD written = 0;
        WriteFile(h, text.c_str(), static_cast<DWORD>(text.size() * sizeof(wchar_t)), &written, nullptr);
    }
}

/// @brief Formats a single file entry for `ls`.
/// @param f WIN32_FIND_DATAW structure containing file info.
/// @param prefix String prefix (used for tree-like recursive output).
/// @param flags Flags affecting output (e.g., verbose, recursive).
/// @return Formatted string representing the file entry.
static std::wstring formatLsEntry(
    const WIN32_FIND_DATAW &f,
    const std::wstring &prefix,
    uint16_t flags)
{
    LARGE_INTEGER size;
    size.LowPart = f.nFileSizeLow;
    size.HighPart = f.nFileSizeHigh;

    if (flags & static_cast<uint16_t>(Flag::VERBOSE))
    {
        return prefix +
               (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? L"d " : L"- ") +
               std::to_wstring(size.QuadPart) + L" " +
               f.cFileName + L"\n";
    }

    return prefix + f.cFileName + L"\n";
}

// ---------------------------------------------------------------------------------------------------------------------------------

namespace FileIO
{

    /**
     * @brief Executes a file command (ls, rew, stats, head, tail, mkdir, rmdir, touch, rm, mv, cp).
     * @param cmd The command type to execute.
     * @param flags Bitwise flags modifying command behavior.
     * @param args Command arguments (file/directory names, counts, etc.).
     * @param ctx Execution context containing handles, pipeline state, redirection state.
     */
    void FileCommands::execute(CommandType cmd, uint16_t flags, const std::vector<std::wstring> &args, Execution::Executor::Context &ctx)
    {
        switch (cmd)
        {
        // LS
        case CommandType::LS:
        {
            std::wstring path = args.empty() ? L"." : args[0];
            executeLS(path, flags, L"", ctx);
            break;
        }

        // REW
        case CommandType::REW:
        {
            if (args.empty())
            {
                writeOut(ctx.stderrHandle, L"Usage: rew <file>\n");
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::write(L"Usage: rew <file>\n");
                break;
            }
            executeREW(args[0], ctx);
            break;
        }

        // STATS
        case CommandType::STATS:
        {
            if (args.empty())
            {
                writeOut(ctx.stderrHandle, L"Usage: stats <file>\n");
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::write(L"Usage: stats <file>\n");
                break;
            }
            executeSTATS(args[0], ctx);
            break;
        }

        // HEAD
        case CommandType::HEAD:
        {
            if (!(flags & FLAG_COUNT) || (!ctx.pipelineEnabled && args.size() < 2))
            {
                writeOut(ctx.stderrHandle, L"Usage: head <file> -n <count>\n");
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::write(L"Usage: head <file> -n <count>\n");
                break;
            }

            size_t count{};
            try
            {
                count = std::stoull(args[1]);
            }
            catch (...)
            {
                writeOut(ctx.stderrHandle, L"Invalid line count\n");
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::write(L"Invalid line count\n");
                break;
            }

            std::wstring filename = ctx.pipelineEnabled ? L"" : args[0];

            auto res = executeHEAD(filename, count, ctx);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::writeln(res.error.message);
            }

            break;
        }

        // TAIL
        case CommandType::TAIL:
        {
            if (!(flags & FLAG_COUNT) || (!ctx.pipelineEnabled && args.size() < 2))
            {
                writeOut(ctx.stderrHandle, L"Usage: tail <file> -n <count>\n");
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::write(L"Usage: tail <file> -n <count>\n");
                break;
            }

            size_t count{};
            try
            {
                count = std::stoull(args[1]);
            }
            catch (...)
            {
                writeOut(ctx.stderrHandle, L"Invalid line count\n");
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::write(L"Invalid line count\n");
                break;
            }

            std::wstring filename = ctx.pipelineEnabled ? L"" : args[0];

            auto res = executeTAIL(filename, count, ctx);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                    console::writeln(res.error.message);
            }

            break;
        }

        // MKDIR
        case CommandType::MKDIR:
        {
            auto res = executeMKDIR(args.empty() ? L"" : args[0]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        // RMDIR
        case CommandType::RMDIR:
        {
            auto res = executeRMDIR(args.empty() ? L"" : args[0]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        // TOUCH
        case CommandType::TOUCH:
        {
            auto res = executeTOUCH(args.empty() ? L"" : args[0]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        // RM
        case CommandType::RM:
        {
            auto res = executeRM(args.empty() ? L"" : args[0]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        // MV
        case CommandType::MV:
        {
            if (args.size() < 2)
            {
                writeOut(ctx.stderrHandle, L"Usage: mv <src> <dst>\n");
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::write(L"Usage: mv <src> <dst>\n");
                break;
            }

            auto res = executeMV(args[0], args[1]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        // CP
        case CommandType::CP:
        {
            if (args.size() < 2)
            {
                writeOut(ctx.stderrHandle, L"Usage: cp <src> <dst>\n");
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::write(L"Usage: cp <src> <dst>\n");
                break;
            }

            auto res = executeCP(args[0], args[1]);
            if (!res.ok())
            {
                writeOut(ctx.stderrHandle, res.error.message);
                if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                    console::writeln(res.error.message);
            }

            break;
        }

        default:
            writeOut(ctx.stderrHandle, L"FileCommands: unsupported command\n");
            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::write(L"FileCommands: unsupported command\n");
            break;
        }
    }

    /// @brief Checks if a given path is a directory.
    /// @param path Path to check.
    /// @return true if the path exists and is a directory, false otherwise.
    bool FileCommands::isDirectory(const std::wstring &path)
    {
        DWORD attr = GetFileAttributesW(path.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES)
            return false;

        return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    /// @brief Copies a single file from source to destination.
    /// @param src Source file path.
    /// @param dst Destination file path.
    /// @return true if the copy succeeds, false otherwise.
    bool FileCommands::copyFile(const std::wstring &src, const std::wstring &dst)
    {
        return CopyFileW(src.c_str(), dst.c_str(), FALSE) != 0; // overwrite allowed
    }

    /// @brief Recursively copies a directory from source to destination.
    /// @param src Source directory.
    /// @param dst Destination directory.
    /// @return true if successful, false otherwise.
    bool FileCommands::copyDirectory(const std::wstring &src, const std::wstring &dst)
    {
        CreateDirectoryW(dst.c_str(), nullptr);

        WIN32_FIND_DATAW ffd;
        std::wstring search = src + L"\\*";

        HANDLE hFind = FindFirstFileW(search.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)
            return false;

        do
        {
            if (wcscmp(ffd.cFileName, L".") == 0 ||
                wcscmp(ffd.cFileName, L"..") == 0)
                continue;

            std::wstring srcPath = src + L"\\" + ffd.cFileName;
            std::wstring dstPath = dst + L"\\" + ffd.cFileName;

            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!copyDirectory(srcPath, dstPath))
                {
                    FindClose(hFind);
                    return false;
                }
            }
            else
            {
                if (!CopyFileW(srcPath.c_str(), dstPath.c_str(), FALSE))
                {
                    FindClose(hFind);
                    return false;
                }
            }

        } while (FindNextFileW(hFind, &ffd));

        FindClose(hFind);
        return true;
    }

    /// @brief Reads and writes the contents of a file (rew command).
    /// @param filename Path to the file.
    /// @param ctx Execution context.
    void FileCommands::executeREW(const std::wstring &filename, Execution::Executor::Context &ctx)
    {
        HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            writeOut(ctx.stderrHandle, L"rew: cannot open file '" + filename + L"'\n");
            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(L"rew: cannot open file '" + filename + L"'\n");
            return;
        }

        char buffer[4096];
        DWORD bytesRead;
        std::wstring outBuffer;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            std::string chunk(buffer, bytesRead);
            outBuffer += unicode::utf8_to_utf16(chunk);

            if (outBuffer.size() > 16384) // 16 KB
            {
                writeOut(ctx.stdoutHandle, outBuffer);
                outBuffer.clear();
            }
        }

        if (!outBuffer.empty())
        {
            writeOut(ctx.stdoutHandle, outBuffer);
            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(outBuffer);
        }

        CloseHandle(hFile);
    }

    /// @brief Lists the contents of a directory (ls command).
    /// @param pathStr Path to list (default is ".").
    /// @param flags Flags affecting output (e.g., recursive, all, verbose).
    /// @param prefix Prefix used for tree-like recursive display.
    /// @param ctx Execution context.
    void FileCommands::executeLS(
        const std::wstring &pathStr,
        uint16_t flags,
        const std::wstring &prefix,
        Execution::Executor::Context &ctx)
    {
        std::wstring path = pathStr.empty() ? L"." : pathStr;
        std::wstring searchPath = path + L"\\*";

        WIN32_FIND_DATAW ffd;
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &ffd);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            writeOut(ctx.stderrHandle, L"ls: cannot access '" + path + L"'\n");

            if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                console::writeln(L"ls: cannot access '" + path + L"'\n");
            return;
        }

        std::vector<WIN32_FIND_DATAW> entries;
        do
        {
            entries.push_back(ffd);
        } while (FindNextFileW(hFind, &ffd));

        FindClose(hFind);

        std::vector<WIN32_FIND_DATAW> visible;
        for (const auto &e : entries)
        {
            std::wstring name = e.cFileName;

            if (name == L"." || name == L"..")
                continue;
            if (!(flags & static_cast<uint16_t>(Flag::ALL)) &&
                (e.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
                continue;

            visible.push_back(e);
        }

        std::wstring outBuffer;

        for (size_t i = 0; i < visible.size(); ++i)
        {
            const auto &f = visible[i];
            std::wstring name = f.cFileName;

            bool isLast = (i == visible.size() - 1);

            std::wstring treePrefix = prefix;
            if (flags & static_cast<uint16_t>(Flag::RECURSIVE))
                treePrefix += isLast ? L"|___" : L"|---";

            outBuffer += formatLsEntry(f, treePrefix, flags);

            if ((flags & static_cast<uint16_t>(Flag::RECURSIVE)) &&
                (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (f.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                    continue;

                std::wstring newPrefix = prefix;
                newPrefix += isLast ? L"    " : L"|   ";

                executeLS(path + L"\\" + name, flags, newPrefix, ctx);
            }
        }

        if (!outBuffer.empty())
        {
            writeOut(ctx.stdoutHandle, outBuffer);

            if (!ctx.pipelineEnabled && !ctx.redirectionEnabled)
                console::writeln(outBuffer);
        }
    }

    /// @brief Prints file statistics (lines, words, bytes, size, timestamps, attributes).
    /// @param filename File path.
    /// @param ctx Execution context.
    void FileCommands::executeSTATS(const std::wstring &filename, Execution::Executor::Context &ctx)
    {
        HANDLE hFile = CreateFileW(
            filename.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            writeOut(ctx.stderrHandle, L"stats: cannot open file '" + filename + L"'\n");

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::write(L"stats: cannot open file '" + filename + L"'\n");
            return;
        }

        DWORD lineCount = 0;
        DWORD wordCount = 0;
        DWORD byteCount = 0;

        BYTE buffer[4096];
        DWORD bytesRead = 0;
        bool inWord = false;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) &&
               bytesRead > 0)
        {
            byteCount += bytesRead;

            for (DWORD i = 0; i < bytesRead; ++i)
            {
                BYTE c = buffer[i];

                if (c == '\n')
                    ++lineCount;

                if (isspace(c))
                    inWord = false;
                else if (!inWord)
                {
                    ++wordCount;
                    inWord = true;
                }
            }
        }

        BY_HANDLE_FILE_INFORMATION info;
        if (!GetFileInformationByHandle(hFile, &info))
        {
            CloseHandle(hFile);
            writeOut(ctx.stderrHandle, L"stats: failed to get file information\n");

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::write(L"stats: failed to get file information\n");

            return;
        }

        CloseHandle(hFile);

        LARGE_INTEGER size;
        size.HighPart = info.nFileSizeHigh;
        size.LowPart = info.nFileSizeLow;

        std::wstring out;
        out.reserve(512);

        out += L"Lines              : " + std::to_wstring(lineCount) + L"\n";
        out += L"Words              : " + std::to_wstring(wordCount) + L"\n";
        out += L"Bytes              : " + std::to_wstring(byteCount) + L"\n";
        out += L"File size          : " + std::to_wstring(size.QuadPart) + L" bytes\n";
        out += L"Attributes         : " +
               helper::attributesToWSTRING(info.dwFileAttributes) + L"\n";
        out += L"File Creation Time : " +
               helper::FileTimeToWString(info.ftCreationTime) + L"\n";
        out += L"Last Access Time   : " +
               helper::FileTimeToWString(info.ftLastAccessTime) + L"\n";
        out += L"Last Write Time    : " +
               helper::FileTimeToWString(info.ftLastWriteTime) + L"\n";

        if (!out.empty())
        {
            writeOut(ctx.stdoutHandle, out);

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(out);
        }
    }

    /// @brief Outputs the first N lines of a file (head command).
    /// @param filename File path.
    /// @param lineCount Number of lines to display.
    /// @param ctx Execution context.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeHEAD(const std::wstring &filename, size_t lineCount, Execution::Executor::Context &ctx)
    {
        HANDLE hFile = INVALID_HANDLE_VALUE;

        if (filename.empty() && ctx.pipelineEnabled)
        {
            hFile = ctx.stdinHandle;   // READ FROM PIPE
        }
        else
        {
            hFile = CreateFileW(
                filename.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
        }

        if (hFile == INVALID_HANDLE_VALUE)
        {
            writeOut(ctx.stderrHandle, L"head: cannot open file\n");

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(L"head: cannot open file\n");

            return {false, makeLastError(L"head")};
        }

        BYTE buffer[4096];
        DWORD bytesRead = 0;
        size_t currentLine = 0;
        bool done = false;
        std::wstring outBuffer;
        std::wstring currentLineStr;

        while (!done && ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            std::string chunk(reinterpret_cast<const char *>(buffer), bytesRead);
            std::wstring wchunk = unicode::utf8_to_utf16(chunk);

            for (wchar_t ch : wchunk)
            {
                currentLineStr.push_back(ch);
                if (ch == L'\n')
                {
                    outBuffer += currentLineStr;
                    currentLineStr.clear();
                    ++currentLine;

                    if (outBuffer.size() > 16384) // 16 KB buffer
                    {
                        writeOut(ctx.stdoutHandle, outBuffer);
                        outBuffer.clear();
                    }

                    if (currentLine >= lineCount)
                    {
                        done = true;
                        break;
                    }
                }
            }
        }

        if (!currentLineStr.empty())
            outBuffer += currentLineStr;

        if (!outBuffer.empty())
        {
            writeOut(ctx.stdoutHandle, outBuffer);

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(outBuffer);
        }

        if (!(filename.empty() && ctx.pipelineEnabled))
            CloseHandle(hFile);
        return {true, {}};
    }

    /// @brief Creates a new empty file (touch command).
    /// @param filename File path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeTOUCH(const std::wstring &filename)
    {
        std::wstring wFilename = filename;

        HANDLE hFile = CreateFileW(
            wFilename.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
            return {false, makeLastError(L"touch")};

        CloseHandle(hFile);
        return {true, {}};
    }

    /// @brief Deletes a file (rm command).
    /// @param path File path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeRM(const std::wstring &path)
    {
        if (!DeleteFileW(path.c_str()))
            return {false, makeLastError(L"rm")};

        return {true, {}};
    }

    /// @brief Creates a new directory (mkdir command).
    /// @param dirname Directory path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeMKDIR(const std::wstring &dirname)
    {
        if (!CreateDirectoryW(
                dirname.c_str(),
                nullptr))
            return {false, makeLastError(L"mkdir")};

        return {true, {}};
    }

    /// @brief Removes an empty directory (rmdir command).
    /// @param dirname Directory path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeRMDIR(const std::wstring &dirname)
    {
        if (!RemoveDirectoryW(
                dirname.c_str()))
            return {false, makeLastError(L"rmdir")};

        return {true, {}};
    }

    /// @brief Moves a file or directory to a new location (mv command).
    /// @param src Source path.
    /// @param dst Destination path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeMV(const std::wstring &src, const std::wstring &dst)
    {
        std::wstring wSrc = src;
        std::wstring wDst = dst;
        if (isDirectory(wDst))
        {
            wDst += L"\\";
            wDst += helper::basename(wSrc);
        }

        if (!MoveFileExW(
                wSrc.c_str(),
                wDst.c_str(),
                MOVEFILE_REPLACE_EXISTING |
                    MOVEFILE_COPY_ALLOWED))
        {
            return {false, makeLastError(L"mv")};
        }

        return {true, {}};
    }

    /// @brief Copies a file or directory (cp command).
    /// @param src Source path.
    /// @param dst Destination path.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeCP(const std::wstring &src, const std::wstring &dst)
    {
        std::wstring wSrc = src;
        std::wstring wDst = dst;
        DWORD attr = GetFileAttributesW(wSrc.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES)
            return {false, makeLastError(L"cp")};

        if (isDirectory(wDst))
        {
            wDst += L"\\";
            wDst += helper::basename(wSrc);
        }

        bool ok;
        if (attr & FILE_ATTRIBUTE_DIRECTORY)
            ok = copyDirectory(wSrc, wDst);
        else
            ok = copyFile(wSrc, wDst);

        if (!ok)
            return {false, makeLastError(L"cp")};

        return {true, {}};
    }

    /// @brief Outputs the last N lines of a file (tail command).
    /// @param filename File path.
    /// @param lineCount Number of lines to display.
    /// @param ctx Execution context.
    /// @return BoolResult indicating success or failure.
    BoolResult FileCommands::executeTAIL(const std::wstring &filename, size_t lineCount, Execution::Executor::Context &ctx)
    {
        HANDLE hFile = INVALID_HANDLE_VALUE;

        if (filename.empty() && ctx.pipelineEnabled)
        {
            hFile = ctx.stdinHandle;
        }
        else
        {
            hFile = CreateFileW(
                filename.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
        }

        if (hFile == INVALID_HANDLE_VALUE)
        {
            writeOut(ctx.stderrHandle, L"tail: cannot open file\n");

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(L"tail: cannot open file\n");

            return {false, makeLastError(L"tail")};
        }

        BYTE buffer[4096];
        DWORD bytesRead = 0;

        std::deque<std::wstring> lineBuffer;
        std::wstring currentLine;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            std::string chunk(reinterpret_cast<const char *>(buffer), bytesRead);
            std::wstring wchunk = unicode::utf8_to_utf16(chunk);

            for (wchar_t ch : wchunk)
            {
                if (ch == L'\n')
                {
                    lineBuffer.push_back(currentLine);
                    if (lineBuffer.size() > lineCount)
                        lineBuffer.pop_front();

                    currentLine.clear();
                }
                else if (ch != L'\r')
                {
                    currentLine.push_back(ch);
                }
            }
        }

        if (!currentLine.empty())
        {
            lineBuffer.push_back(currentLine);
            if (lineBuffer.size() > lineCount)
                lineBuffer.pop_front();
        }

        if (!(filename.empty() && ctx.pipelineEnabled))
            CloseHandle(hFile);

        std::wstring outBuffer;
        for (const auto &line : lineBuffer)
        {
            outBuffer += line + L"\n";

            // 16 KB
            if (outBuffer.size() > 16384)
            {
                writeOut(ctx.stdoutHandle, outBuffer);
                outBuffer.clear();
            }
        }

        if (!outBuffer.empty())
        {
            writeOut(ctx.stdoutHandle, outBuffer);

            if (ctx.pipelineEnabled == false && ctx.redirectionEnabled == false)
                console::writeln(outBuffer);
        }

        return {true, {}};
    }
}