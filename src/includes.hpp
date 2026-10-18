#pragma once

#include <windows.h>
#include <unordered_set>
#include <array>
#include <cstdint>
#include "gd/gd.hpp"

// Global game window handle
extern HWND hwnd;
extern uintptr_t gd_base;
extern gd::PlayLayer* g_currentPlayLayer;

// Menu visibility
extern bool menuVisible;

// Visual options
extern bool noDeathEffect;
extern bool hideEndscreen;
extern bool hideNewBest;
extern bool noEffect;

extern bool fadeLevel;
extern double fadeLevelInDuration;
extern double fadeLevelOutDuration;

extern bool fadeAudio;
extern double fadeAudioInDuration;
extern double fadeAudioOutDuration;

// Gameplay: Straight Fly
extern bool straightFly;
extern bool straightFlyP1;
extern bool straightFlyP2;
extern double straightFlyThresholdP1;
extern double straightFlyThresholdP2;

// Gameplay: Straight UFO
extern bool straightUfo;
extern bool straightUfoP1;
extern bool straightUfoP2;
extern double straightUfoTargetP1;
extern double straightUfoTargetP2;
extern double straightUfoThresholdP1;
extern double straightUfoThresholdP2;

// Layout Mode colors
extern gd::ccColor3B layoutModeColorBackground;
extern gd::ccColor3B layoutModeColorGround;

// Flip on Death
extern int flipPlayer;
extern bool flipOnDeath;
extern bool flipOnDeathBoth;
extern bool flipOnDeathP1;
extern bool flipOnDeathP2;
extern bool flipOnDeathLogicP1;
extern bool flipOnDeathLogicP2;
extern bool flipOnDeathSwift;
extern bool flipOnDeathUnfreeze;

// Automatic clicks
extern bool autoUnfreeze;
extern bool autoSwift;
extern bool extraClick;
extern int  extraClickAmount;

extern bool clickGreenDash;
extern bool clickBlackOrbs;
extern bool clickJumpPads;
extern bool clickGravityPads;
extern bool clickedJumpPad;

// Gravity
extern bool maintainGravity;

// Autoclicker
extern bool autoclickerP1;
extern bool autoclickerP2;
extern int autoclickerEveryP1;
extern int autoclickerEveryP2;
extern int autoclickerHoldP1;
extern int autoclickerHoldP2;
extern int autoclickerTimerP1;
extern int autoclickerTimerP2;
extern bool autoclickerHoldingP1;
extern bool autoclickerHoldingP2;
extern bool autoclickerSwiftP1;
extern bool autoclickerSwiftP2;

// Noclip
extern bool noclip;
extern bool noclipP1;
extern bool noclipP2;

// Other
extern bool spamCheckpoints;
extern bool restartFirstFrame;
extern bool layoutMode;
extern bool blackOrbUfo;

extern bool releaseGravityOrbsPrevent;
extern bool optimizeStackedOrbs;
extern bool optimizeRingJump;
extern bool optimizeStartDashing;
extern bool optimizeStopDashing;

// Color helpers
std::array<float, 3> colorToFloat(gd::ccColor3B color);
gd::ccColor3B floatToColor(const float* col);

// Filter sets for Layout Mode
extern const std::unordered_set<int> decoration;
extern const std::unordered_set<int> filter;
