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

// FILE: src\platform\AppDataPath.cpp
// PURPOSE: Creates AppData\Roaming\esh for storing esh-related stuff.

// INCLUDE LIBRARIES

#include <stdexcept>
#include <filesystem>

#include <windows.h>
#include <shlobj.h>

#include "AppDataPath.hpp"

namespace Platform
{
    /* Create an AppData\Roaming\esh directory if there is not. */

    static std::filesystem::path queryRoamingAppData()
    {
        PWSTR rawPath = nullptr;

        HRESULT hr = SHGetKnownFolderPath(
            FOLDERID_RoamingAppData,
            0,
            nullptr,
            &rawPath);

        if (FAILED(hr))
            throw std::runtime_error("Failed to get Roaming AppData path");

        std::filesystem::path path(rawPath);

        CoTaskMemFree(rawPath);

        return path;
    }

    std::filesystem::path getBasePath()
    {
        static std::filesystem::path base = queryRoamingAppData() / L"esh";

        return base;
    }

    void init()
    {
        const auto &path = getBasePath();

        if (!std::filesystem::exists(path))
        {
            std::filesystem::create_directories(path);
        }
    }
}
