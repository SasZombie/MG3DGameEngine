#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "Asset.hpp"
namespace sas
{
    namespace script
    {
        struct ScriptInstance
        {
        #ifdef _WIN32
            HMODULE handle = nullptr;
        #else
            void *handle = nullptr;
        #endif
            void *instance = nullptr;

            using CreateFn = void *(*)() noexcept;
            using DestroyFn = void (*)(void *) noexcept;
            using UpdateFn = void (*)(void *, sas::Asset &) noexcept;

            CreateFn createFn = nullptr;
            DestroyFn destroyFn = nullptr;
            UpdateFn updateFn = nullptr;

            ~ScriptInstance() noexcept { unload(); }

            bool load(const std::string &dllPath) noexcept;

            void unload() noexcept;

            void callUpdate(sas::Asset &obj) noexcept;
        };

        std::string createDLL(const std::string &filePath, const std::string &outPath) noexcept;

    } // namespace script

} // namespace sas