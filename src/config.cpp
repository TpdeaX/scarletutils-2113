#include "config.hpp"
#include "includes.hpp"
#include <windows.h>
#include <string>

static std::string getConfigPath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos) {
        path = path.substr(0, pos + 1);
    }
    path += "scarletutils_2113.ini";
    return path;
}

namespace Config {

void load() {
    std::string path = getConfigPath();

    auto readBool = [&](const char* key, bool def) -> bool {
        return GetPrivateProfileIntA("Settings", key, def ? 1 : 0, path.c_str()) != 0;
    };
    auto readDouble = [&](const char* key, double def) -> double {
        char buf[64];
        char defBuf[64];
        snprintf(defBuf, sizeof(defBuf), "%.6f", def);
        GetPrivateProfileStringA("Settings", key, defBuf, buf, sizeof(buf), path.c_str());
        return std::atof(buf);
    };

    noDeathEffect = readBool("noDeathEffect", false);
    hideEndscreen = readBool("hideEndscreen", false);
    hideNewBest = readBool("hideNewBest", false);
    noEffect = readBool("noEffect", false);

    fadeLevel = readBool("fadeLevel", false);
    fadeLevelInDuration = readDouble("fadeLevelInDuration", 0.5);
    fadeLevelOutDuration = readDouble("fadeLevelOutDuration", 1.0);

    fadeAudio = readBool("fadeAudio", false);
    fadeAudioInDuration = readDouble("fadeAudioInDuration", 0.5);
    fadeAudioOutDuration = readDouble("fadeAudioOutDuration", 1.0);

    straightFlyP1 = readBool("straightFlyP1", false);
    straightFlyP2 = readBool("straightFlyP2", false);
    straightFlyThresholdP1 = readDouble("straightFlyThresholdP1", 0.0);
    straightFlyThresholdP2 = readDouble("straightFlyThresholdP2", 0.0);

    straightUfoP1 = readBool("straightUfoP1", false);
    straightUfoP2 = readBool("straightUfoP2", false);
    straightUfoTargetP1 = readDouble("straightUfoTargetP1", 135.0);
    straightUfoTargetP2 = readDouble("straightUfoTargetP2", 135.0);
    straightUfoThresholdP1 = readDouble("straightUfoThresholdP1", 0.0);
    straightUfoThresholdP2 = readDouble("straightUfoThresholdP2", 0.0);

    optimizeStackedOrbs = readBool("optimizeStackedOrbs", false);
}

void save() {
    std::string path = getConfigPath();

    auto writeBool = [&](const char* key, bool val) {
        WritePrivateProfileStringA("Settings", key, val ? "1" : "0", path.c_str());
    };
    auto writeDouble = [&](const char* key, double val) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%.6f", val);
        WritePrivateProfileStringA("Settings", key, buf, path.c_str());
    };

    writeBool("noDeathEffect", noDeathEffect);
    writeBool("hideEndscreen", hideEndscreen);
    writeBool("hideNewBest", hideNewBest);
    writeBool("noEffect", noEffect);

    writeBool("fadeLevel", fadeLevel);
    writeDouble("fadeLevelInDuration", fadeLevelInDuration);
    writeDouble("fadeLevelOutDuration", fadeLevelOutDuration);

    writeBool("fadeAudio", fadeAudio);
    writeDouble("fadeAudioInDuration", fadeAudioInDuration);
    writeDouble("fadeAudioOutDuration", fadeAudioOutDuration);

    writeBool("straightFlyP1", straightFlyP1);
    writeBool("straightFlyP2", straightFlyP2);
    writeDouble("straightFlyThresholdP1", straightFlyThresholdP1);
    writeDouble("straightFlyThresholdP2", straightFlyThresholdP2);

    writeBool("straightUfoP1", straightUfoP1);
    writeBool("straightUfoP2", straightUfoP2);
    writeDouble("straightUfoTargetP1", straightUfoTargetP1);
    writeDouble("straightUfoTargetP2", straightUfoTargetP2);
    writeDouble("straightUfoThresholdP1", straightUfoThresholdP1);
    writeDouble("straightUfoThresholdP2", straightUfoThresholdP2);

    writeBool("optimizeStackedOrbs", optimizeStackedOrbs);
}

} // namespace Config
