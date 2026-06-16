#pragma once
#ifdef __ANDROID__

#include "ApiSystem.h"

/**
 * Android implementation of ApiSystem.
 * All Batocera/Linux shell-script operations are no-ops or stubs.
 * This will be fleshed out incrementally as features are ported.
 */
class AndroidApiSystem : public ApiSystem
{
public:
    AndroidApiSystem() = default;

    bool isScriptingSupported(ScriptId script) override;

    std::string getVersion(bool extra = false) override;
    std::string getApplicationName() override;

    unsigned long getFreeSpaceGB(std::string mountpoint) override;
    std::string getFreeSpaceUserInfo() override;
    std::string getFreeSpaceSystemInfo() override;

    std::string getIpAddress() override;

    // No shell scripts on Android — return empty/false for everything
    std::pair<std::string, int> getLatestVersion() override { return {"", 0}; }
    bool canUpdate(std::vector<std::string>& output) override { return false; }
    void updateSystem(const std::function<void(const std::string)>& fn) override {}
    bool ping() override { return false; }

    std::vector<std::string> getSystemInformations() override;
    std::vector<std::string> getAvailableStorageDevices() override { return {}; }
    std::vector<std::string> getVideoModes() override { return {}; }
    std::vector<BiosSystem> getBiosInformations(const std::string& sys = "") override { return {}; }

    bool generateSupportFile() override { return false; }
    bool launchKodi(Window* w) override { return false; }

    std::vector<std::string> getNetworkInterfaces() override { return {}; }
    bool setVolume(int vol, std::function<void(const std::string)> func) override { return false; }
    int getVolume() override { return 50; }
    std::string getAudioDevice() override { return ""; }

    std::vector<std::string> getAvailableAudioOutputDevices() override { return {}; }
    std::vector<std::string> getAvailableAudioInputDevices() override { return {}; }

    std::string getCurrentAudioOutputDevice() override { return ""; }
    std::string getCurrentAudioInputDevice() override { return ""; }

    bool setAudioOutputDevice(const std::string& dev) override { return false; }
    bool setAudioInputDevice(const std::string& dev) override { return false; }
};

#endif // __ANDROID__
