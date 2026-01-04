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

#include <vector>
#include <string>
#include <iostream>

#include <windows.h>

#include "../headers/Engine.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Console.hpp"
#include "../headers/Unicode.hpp"
#include "../headers/Helper.hpp"
#include "FileCommands.hpp"

namespace FileIO
{

    void FileCommands::execute(CommandType cmd, uint8_t flags, const std::vector<std::wstring> &args)
    {
        // Helper lambda to print boolean command results
        auto printBoolResult =
            [](const BoolResult &res, const std::wstring &successMsg)
        {
            if (res.ok())
            {
                console::setColor(ConsoleColor::Green);
                console::writeln(successMsg);
            }
            else
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << res.error.message << std::endl;
            }
            console::reset();
        };

        switch (cmd)
        {
        case CommandType::LS:
            // List directory contents
            {
                std::wstring path = args.empty() ? L"." : args[0];
                executeLS(path, flags);
                break;
            }
        case CommandType::REW:
        {
            // Review the contents of a file
            if (args.empty())
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << L"Usage: rew <file>" << std::endl;
                console::reset();
                break;
            }
            executeREW(args[0]);
            break;
        }

        case CommandType::STATS:
            // Show detailed information about a file
            {
                if (args.empty())
                {
                    console::setColor(ConsoleColor::Red);
                    std::wcerr << L"Usage: stats <file>" << std::endl;
                    console::reset();
                    break;
                }

                auto res = executeSTATS(args[0]);
                if (res.ok())
                {
                    console::writeln(res.value);
                }
                else
                {
                    console::setColor(ConsoleColor::Red);
                    std::wcerr << res.error.message << std::endl;
                    console::reset();
                }
                break;
            }

        case CommandType::HEAD:
        {
            // Get contents from a file using line count
            if (args.empty() || !(flags & FLAG_COUNT) || args.size() < 2)
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << L"Usage: head <file> -n <line count>" << std::endl;
                console::reset();
                break;
            }

            size_t lineCount = 0;
            try
            {
                lineCount = static_cast<size_t>(std::stoull(args[1])); // string → size_t
            }
            catch (const std::exception &e)
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << L"Invalid line count: " << args[1] << std::endl;
                console::reset();
                break;
            }

            auto res = executeHEAD(args[0], lineCount);
            if (res.ok())
            {
                console::writeln(res.value);
            }
            else
            {
                console::setColor(ConsoleColor::Red);
                std::wcerr << res.error.message << std::endl;
                console::reset();
            }
            break;
        }

        case CommandType::MV:
        {
            // Move (rename) a file or directory
            if (args.size() < 2)
            {
                makeLastError(L"mv");
                break;
            }

            executeMV(args[0], args[1]);
            console::writeln(L"Move operation successful.");
            break;
        }

        case CommandType::CP:
        {
            // Copy a file or directory
            if (args.size() < 2)
            {
                makeLastError(L"cp");
                break;
            }

            executeCP(args[0], args[1]);
            console::writeln(L"Copy operation successful.");
            break;
        }

        case CommandType::MKDIR:
            // Create a new directory
            printBoolResult(executeMKDIR(args.empty() ? L"" : args[0]), L"Directory created.");
            break;

        case CommandType::RMDIR:
            // Remove an empty directory
            printBoolResult(executeRMDIR(args.empty() ? L"" : args[0]), L"Directory removed.");
            break;

        case CommandType::TOUCH:
            // Create a new empty file
            printBoolResult(executeTOUCH(args.empty() ? L"" : args[0]), L"File created.");
            break;

        case CommandType::RM:
            // Remove a file
            printBoolResult(executeRM(args.empty() ? L"" : args[0]), L"File deleted.");
            break;

        default:
            console::setColor(ConsoleColor::Red);
            std::wcerr << L"FileCommands: Unsupported command" << std::endl;
            console::reset();
            break;
        }
    }

    // Check if the file is a directory.
    bool FileCommands::isDirectory(const std::wstring &path)
    {
        DWORD attr = GetFileAttributesW(path.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES)
            return false;

        return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    // Copy File Function
    bool FileCommands::copyFile(const std::wstring &src, const std::wstring &dst)
    {
        return CopyFileW(src.c_str(), dst.c_str(), FALSE) != 0; // overwrite allowed
    }

    // Copy Directory Function
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

    // REW COMMAND
    void FileCommands::executeREW(const std::wstring &filename)
    {
        std::wstring wFilename = filename;

        HANDLE hFile = CreateFileW(
            wFilename.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            console::setColor(ConsoleColor::Red);
            makeLastError(L"rew");
            console::write(L"rew: cannot open file '" + wFilename + L"'\n");
            makeLastError(L"rew");
            console::reset();
            return;
        }

        char buffer[4096];
        DWORD bytesRead;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            std::string chunk(buffer, bytesRead);
            std::wstring w = unicode::utf8_to_utf16(chunk);
            console::write(w);
        }

        CloseHandle(hFile);
    }

    // LS COMMAND
    void FileCommands::executeLS(const std::wstring &pathStr, uint8_t flags, const std::wstring &prefix)
    {
        std::wstring searchPath = pathStr + L"\\*";

        WIN32_FIND_DATAW ffd;
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &ffd);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            std::wcerr << L"ls: cannot access '" << pathStr << L"'\n";
            return;
        }

        std::vector<WIN32_FIND_DATAW> entries;

        do
        {
            entries.push_back(ffd);
        } while (FindNextFileW(hFind, &ffd));

        FindClose(hFind);

        for (size_t i = 0; i < entries.size(); ++i)
        {
            const auto &f = entries[i];
            std::wstring name = f.cFileName;

            if (name == L"." || name == L"..")
                continue;

            if (!(flags & static_cast<uint8_t>(Flag::ALL)) &&
                (f.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
                continue;

            bool isLast = (i == entries.size() - 1);

            std::wstring treePrefix = prefix;
            if (flags & static_cast<uint8_t>(Flag::RECURSIVE))
            {
                treePrefix += isLast ? L"|___" : L"|---";
            }

            printLsEntry(f, treePrefix, flags);

            if ((flags & static_cast<uint8_t>(Flag::RECURSIVE)) &&
                (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::wstring newPrefix = prefix;
                newPrefix += isLast ? L"    " : L"|   ";

                executeLS(pathStr + L"\\" + name, flags, newPrefix);
            }
        }
    }

    void FileCommands::printLsEntry(const WIN32_FIND_DATAW &f, const std::wstring &prefix, uint8_t flags)
    {
        DWORD attrs = f.dwFileAttributes;

        helper::setColorByAttributes(attrs);

        LARGE_INTEGER size;
        size.LowPart = f.nFileSizeLow;
        size.HighPart = f.nFileSizeHigh;

        if (flags & static_cast<uint8_t>(Flag::VERBOSE))
        {
            console::write(prefix);
            console::write(attrs & FILE_ATTRIBUTE_DIRECTORY ? L"d " : L"- ");
            console::write(std::to_wstring(size.QuadPart) + L" ");
            console::writeln(f.cFileName);
        }
        else
        {
            console::write(prefix);
            console::writeln(f.cFileName);
        }

        console::reset();
    }

    // STATS COMMAND
    Result<std::wstring> FileCommands::executeSTATS(const std::wstring &filename)
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
            return {L"", makeLastError(L"stats")};

        DWORD lineCount = 0;
        DWORD wordCount = 0;
        DWORD byteCount = 0;

        BYTE buffer[4096];
        DWORD bytesRead;
        bool inWord = false;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            byteCount += bytesRead;
            for (DWORD i = 0; i < bytesRead; ++i)
            {
                BYTE c = buffer[i];
                if (c == '\n')
                    lineCount++;
                if (isspace(c))
                    inWord = false;
                else if (!inWord)
                {
                    wordCount++;
                    inWord = true;
                }
            }
        }

        BY_HANDLE_FILE_INFORMATION info;
        if (!GetFileInformationByHandle(hFile, &info))
        {
            CloseHandle(hFile);
            return {L"", makeLastError(L"stats")};
        }

        FILETIME fileCreationTime = info.ftCreationTime;
        FILETIME lastAccessTime = info.ftLastAccessTime;
        FILETIME lastWriteTime = info.ftLastWriteTime;

        CloseHandle(hFile);

        LARGE_INTEGER size;
        size.HighPart = info.nFileSizeHigh;
        size.LowPart = info.nFileSizeLow;

        std::wstring result;
        result += L"Lines              : " + std::to_wstring(lineCount) + L"\n";
        result += L"Words              : " + std::to_wstring(wordCount) + L"\n";
        result += L"Bytes              : " + std::to_wstring(byteCount) + L"\n";
        result += L"File size          : " + std::to_wstring(size.QuadPart) + L" bytes\n";
        result += L"Attributes         : " + helper::attributesToWSTRING(info.dwFileAttributes) + L"\n";
        result += L"File Creation Time : " + helper::FileTimeToWString(fileCreationTime) + L"\n";
        result += L"Last Access Time   : " + helper::FileTimeToWString(lastAccessTime) + L"\n";
        result += L"Last Write Time    : " + helper::FileTimeToWString(lastWriteTime) + L"\n";

        return {result, {}};
    }

    // HEAD COMMAND
    Result<std::wstring> FileCommands::executeHEAD(const std::wstring &filename, size_t lineCount)
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
            return {L"", makeLastError(L"head")};

        BYTE buffer[4096];
        DWORD bytesRead;
        bool done = false;
        size_t currentLine = 0;
        std::wstring result;

        while (!done && ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
        {
            for (DWORD i = 0; i < bytesRead; ++i)
            {
                wchar_t c = buffer[i];

                result += c;

                if (c == L'\n')
                {
                    currentLine++;
                    if (currentLine >= lineCount)
                    {
                        done = true;
                        break;
                    }
                }
            }
        }

        CloseHandle(hFile);
        return {result, {}};
    }

    // TOUCH COMMAND
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

    // RM COMMAND
    BoolResult FileCommands::executeRM(const std::wstring &path)
    {
        if (!DeleteFileW(path.c_str()))
            return {false, makeLastError(L"rm")};

        return {true, {}};
    }

    // MKDIR COMMAND
    BoolResult FileCommands::executeMKDIR(const std::wstring &dirname)
    {
        if (!CreateDirectoryW(
                dirname.c_str(),
                nullptr))
            return {false, makeLastError(L"mkdir")};

        return {true, {}};
    }

    // RMDIR COMMAND
    BoolResult FileCommands::executeRMDIR(const std::wstring &dirname)
    {
        if (!RemoveDirectoryW(
                dirname.c_str()))
            return {false, makeLastError(L"rmdir")};

        return {true, {}};
    }

    // MV COMMAND
    BoolResult FileCommands::executeMV(const std::wstring &src,
                                       const std::wstring &dst)
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

    // CP COMMAND
    BoolResult FileCommands::executeCP(const std::wstring &src,
                                       const std::wstring &dst)
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
}