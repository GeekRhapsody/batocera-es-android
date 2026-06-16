#ifdef __ANDROID__
#include "AndroidApiSystem.h"
#include "Log.h"
#include "Paths.h"
#include "utils/FileSystemUtil.h"
#include <SDL.h>

bool AndroidApiSystem::isScriptingSupported(ScriptId script)
{
    // No Batocera shell scripts on Android
    return false;
}

std::string AndroidApiSystem::getVersion(bool extra)
{
    return "Android";
}

std::string AndroidApiSystem::getApplicationName()
{
    return "EmulationStation";
}

unsigned long AndroidApiSystem::getFreeSpaceGB(std::string mountpoint)
{
    // Return 0 for now; could query StatFs via JNI later
    return 0;
}

std::string AndroidApiSystem::getFreeSpaceUserInfo()
{
    return "";
}

std::string AndroidApiSystem::getFreeSpaceSystemInfo()
{
    return "";
}

std::string AndroidApiSystem::getIpAddress()
{
    // SDL2 does not expose IP directly; return a placeholder
    return "127.0.0.1";
}

std::vector<std::string> AndroidApiSystem::getSystemInformations()
{
    std::vector<std::string> info;
    info.push_back("Platform: Android");
    info.push_back("ABI: arm64-v8a");
    const char* internalPath = SDL_AndroidGetInternalStoragePath();
    if (internalPath)
        info.push_back(std::string("Internal: ") + internalPath);
    return info;
}

#endif // __ANDROID__
