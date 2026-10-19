#include "config.hpp"
#include "includes.hpp"
#include <windows.h>
#include <string>
#include <cstdio>
#include <cstdlib>

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
    auto readInt = [&](const char* key, int def) -> int {
        return GetPrivateProfileIntA("Settings", key, def, path.c_str());
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

    straightFly = readBool("straightFly", false);
    straightFlyP1 = readBool("straightFlyP1", false);
    straightFlyP2 = readBool("straightFlyP2", false);
    straightFlyThresholdP1 = readDouble("straightFlyThresholdP1", 0.0);
    straightFlyThresholdP2 = readDouble("straightFlyThresholdP2", 0.0);

    straightUfo = readBool("straightUfo", false);
    straightUfoP1 = readBool("straightUfoP1", false);
    straightUfoP2 = readBool("straightUfoP2", false);
    straightUfoTargetP1 = readDouble("straightUfoTargetP1", 135.0);
    straightUfoTargetP2 = readDouble("straightUfoTargetP2", 135.0);
    straightUfoThresholdP1 = readDouble("straightUfoThresholdP1", 0.0);
    straightUfoThresholdP2 = readDouble("straightUfoThresholdP2", 0.0);

    optimizeStackedOrbs = readBool("optimizeStackedOrbs", false);

    layoutMode = readBool("layoutMode", false);
    layoutModeColorBackground.r = static_cast<uint8_t>(readInt("layoutBgR", 160));
    layoutModeColorBackground.g = static_cast<uint8_t>(readInt("layoutBgG", 160));
    layoutModeColorBackground.b = static_cast<uint8_t>(readInt("layoutBgB", 160));
    layoutModeColorGround.r = static_cast<uint8_t>(readInt("layoutGroundR", 160));
    layoutModeColorGround.g = static_cast<uint8_t>(readInt("layoutGroundG", 160));
    layoutModeColorGround.b = static_cast<uint8_t>(readInt("layoutGroundB", 160));

    noclip = readBool("noclip", false);
    noclipP1 = readBool("noclipP1", true);
    noclipP2 = readBool("noclipP2", true);

    flipOnDeath = readBool("flipOnDeath", false);
    flipOnDeathBoth = readBool("flipOnDeathBoth", false);
    flipOnDeathP1 = readBool("flipOnDeathP1", true);
    flipOnDeathP2 = readBool("flipOnDeathP2", true);
    flipOnDeathSwift = readBool("flipOnDeathSwift", false);
    flipOnDeathUnfreeze = readBool("flipOnDeathUnfreeze", false);

    clickGreenDash = readBool("clickGreenDash", false);
    clickBlackOrbs = readBool("clickBlackOrbs", false);
    blackOrbUfo = readBool("blackOrbUfo", false);
    clickJumpPads = readBool("clickJumpPads", false);
    clickGravityPads = readBool("clickGravityPads", false);

    maintainGravity = readBool("maintainGravity", false);

    autoclickerP1 = readBool("autoclickerP1", false);
    autoclickerP2 = readBool("autoclickerP2", false);
    autoclickerEveryP1 = readInt("autoclickerEveryP1", 1);
    autoclickerEveryP2 = readInt("autoclickerEveryP2", 1);
    autoclickerHoldP1 = readInt("autoclickerHoldP1", 1);
    autoclickerHoldP2 = readInt("autoclickerHoldP2", 1);
    autoclickerSwiftP1 = readBool("autoclickerSwiftP1", false);
    autoclickerSwiftP2 = readBool("autoclickerSwiftP2", false);

    autoSwift = readBool("autoSwift", false);
    extraClick = readBool("extraClick", false);
    extraClickAmount = readInt("extraClickAmount", 1);

    restartFirstFrame = readBool("restartFirstFrame", false);
}

void save() {
    std::string path = getConfigPath();

    auto writeBool = [&](const char* key, bool val) {
        WritePrivateProfileStringA("Settings", key, val ? "1" : "0", path.c_str());
    };
    auto writeInt = [&](const char* key, int val) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", val);
        WritePrivateProfileStringA("Settings", key, buf, path.c_str());
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

    writeBool("straightFly", straightFly);
    writeBool("straightFlyP1", straightFlyP1);
    writeBool("straightFlyP2", straightFlyP2);
    writeDouble("straightFlyThresholdP1", straightFlyThresholdP1);
    writeDouble("straightFlyThresholdP2", straightFlyThresholdP2);

    writeBool("straightUfo", straightUfo);
    writeBool("straightUfoP1", straightUfoP1);
    writeBool("straightUfoP2", straightUfoP2);
    writeDouble("straightUfoTargetP1", straightUfoTargetP1);
    writeDouble("straightUfoTargetP2", straightUfoTargetP2);
    writeDouble("straightUfoThresholdP1", straightUfoThresholdP1);
    writeDouble("straightUfoThresholdP2", straightUfoThresholdP2);

    writeBool("optimizeStackedOrbs", optimizeStackedOrbs);

    writeBool("layoutMode", layoutMode);
    writeInt("layoutBgR", layoutModeColorBackground.r);
    writeInt("layoutBgG", layoutModeColorBackground.g);
    writeInt("layoutBgB", layoutModeColorBackground.b);
    writeInt("layoutGroundR", layoutModeColorGround.r);
    writeInt("layoutGroundG", layoutModeColorGround.g);
    writeInt("layoutGroundB", layoutModeColorGround.b);

    writeBool("noclip", noclip);
    writeBool("noclipP1", noclipP1);
    writeBool("noclipP2", noclipP2);

    writeBool("flipOnDeath", flipOnDeath);
    writeBool("flipOnDeathBoth", flipOnDeathBoth);
    writeBool("flipOnDeathP1", flipOnDeathP1);
    writeBool("flipOnDeathP2", flipOnDeathP2);
    writeBool("flipOnDeathSwift", flipOnDeathSwift);
    writeBool("flipOnDeathUnfreeze", flipOnDeathUnfreeze);

    writeBool("clickGreenDash", clickGreenDash);
    writeBool("clickBlackOrbs", clickBlackOrbs);
    writeBool("blackOrbUfo", blackOrbUfo);
    writeBool("clickJumpPads", clickJumpPads);
    writeBool("clickGravityPads", clickGravityPads);

    writeBool("maintainGravity", maintainGravity);

    writeBool("autoclickerP1", autoclickerP1);
    writeBool("autoclickerP2", autoclickerP2);
    writeInt("autoclickerEveryP1", autoclickerEveryP1);
    writeInt("autoclickerEveryP2", autoclickerEveryP2);
    writeInt("autoclickerHoldP1", autoclickerHoldP1);
    writeInt("autoclickerHoldP2", autoclickerHoldP2);
    writeBool("autoclickerSwiftP1", autoclickerSwiftP1);
    writeBool("autoclickerSwiftP2", autoclickerSwiftP2);

    writeBool("autoSwift", autoSwift);
    writeBool("extraClick", extraClick);
    writeInt("extraClickAmount", extraClickAmount);

    writeBool("restartFirstFrame", restartFirstFrame);
}

} // namespace Config
