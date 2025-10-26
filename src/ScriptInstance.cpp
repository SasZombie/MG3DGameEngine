#include "ScriptInstance.hpp"

std::string sas::script::createDLL(const std::string &filePath, const std::string &outPath) noexcept
{

    const std::string dllFile =
#ifdef _WIN32
        "ScriptsShared/" + outPath + ".dll";
#else
        "ScriptsShared/" + outPath + ".so";
#endif

#ifdef _WIN32
    system(("cl /LD " + filePath + " /I include /Fe:" + dllFile).c_str());
#else
    system(("g++ -shared -fPIC " + filePath + " -I../include/GameEngine -o " + dllFile).c_str());
#endif

    return dllFile;
}

bool sas::script::ScriptInstance::load(const std::string &dllPath) noexcept
{

#ifdef _WIN32
    handle = LoadLibraryA(dllPath.c_str());
    if (!handle)
        return false;
    createFn = (CreateFn)GetProcAddress(handle, "create");
    destroyFn = (DestroyFn)GetProcAddress(handle, "destroy");
    updateFn = (UpdateFn)GetProcAddress(handle, "update");
#else
    handle = dlopen(dllPath.c_str(), RTLD_LAZY);
    if (!handle)
        return false;
    createFn = (CreateFn)dlsym(handle, "create");
    destroyFn = (DestroyFn)dlsym(handle, "destroy");
    updateFn = (UpdateFn)dlsym(handle, "update");
#endif
    if (!createFn || !destroyFn)
        return false;

    if (!updateFn)
    {
        std::cout << "Cant find update\n";
    }

    instance = createFn();
    return instance != nullptr;
}

void sas::script::ScriptInstance::unload() noexcept
{

    if (instance && destroyFn)
        destroyFn(instance);
#ifdef _WIN32
    if (handle)
        FreeLibrary(handle);
#else
    if (handle)
        dlclose(handle);
#endif
    handle = nullptr;
    instance = nullptr;
}

void sas::script::ScriptInstance::callUpdate(sas::Asset &obj) noexcept
{
    if (updateFn && instance)
        updateFn(instance, obj);
}
