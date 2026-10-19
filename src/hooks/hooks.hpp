#pragma once

#include <windows.h>
#include <cstdint>
#include "gd/gd.hpp"

namespace Hooks {
    // GD 2.113 Win32 Memory Offsets (relative to base address)
    constexpr uintptr_t OFFSET_BGL_PUSH_BUTTON        = 0x111500;
    constexpr uintptr_t OFFSET_BGL_RELEASE_BUTTON     = 0x111660;
    constexpr uintptr_t OFFSET_BGL_BUMP_PLAYER         = 0x10ED50;

    constexpr uintptr_t OFFSET_PLAYLAYER_INIT         = 0x1FB780;
    constexpr uintptr_t OFFSET_PLAYLAYER_DTOR         = 0x1FAFC0;
    constexpr uintptr_t OFFSET_PLAYLAYER_UPDATE       = 0x2029C0;
    constexpr uintptr_t OFFSET_PLAYLAYER_ADDOBJECT    = 0x2017E0;
    constexpr uintptr_t OFFSET_PLAYLAYER_DESTROYPLAYER = 0x20A1A0;
    constexpr uintptr_t OFFSET_PLAYLAYER_RESETLEVEL   = 0x20BF00;
    constexpr uintptr_t OFFSET_PLAYLAYER_STARTGAME    = 0x1FD390;
    constexpr uintptr_t OFFSET_PLAYLAYER_SHOWENDLAYER = 0x1FFD00;
    constexpr uintptr_t OFFSET_PLAYLAYER_SHOWNEWBEST  = 0x1FE3A0;
    constexpr uintptr_t OFFSET_PLAYLAYER_GRAVITYEFF   = 0x207E00;
    constexpr uintptr_t OFFSET_PLAYLAYER_TRANSITION   = 0x20DBD0;
    constexpr uintptr_t OFFSET_PLAYLAYER_TOGGLEGLITTER = 0x20A0D0;
    constexpr uintptr_t OFFSET_PLAYLAYER_UPDATECOLOR  = 0x207070;

    constexpr uintptr_t OFFSET_PLAYER_SPAWNPORTALCIRC = 0x1EF680;
    constexpr uintptr_t OFFSET_PLAYER_PLAYDEATHEFF    = 0x1EFBE0;
    constexpr uintptr_t OFFSET_PLAYER_STOPDASHING     = 0x1EE6E0;
    constexpr uintptr_t OFFSET_PLAYER_PROPELLPLAYER   = 0x1F8B50;

    constexpr uintptr_t OFFSET_GAMEOBJ_ADDGLOW        = 0xCFEF0;
    constexpr uintptr_t OFFSET_GAMEOBJ_PLAYSHINEEFF   = 0xEAB20;
    constexpr uintptr_t OFFSET_GAMEOBJ_SETVISIBLE     = 0xE57C0;

    constexpr uintptr_t OFFSET_RINGOBJ_SPAWNCIRCLE    = 0x2523A0;
    constexpr uintptr_t OFFSET_CIRCLEWAVE_DRAW        = 0x17100;

    constexpr uintptr_t OFFSET_PAUSELAYER_CUSTOMSETUP = 0x1E4620;
    constexpr uintptr_t OFFSET_EFFECTMANAGER_UPDATECOLORS = 0x11C660;
    constexpr uintptr_t OFFSET_GROUND_UPDATECOLOR01        = 0x12DB40;
    constexpr uintptr_t OFFSET_GROUND_UPDATECOLOR02        = 0x12DBA0;
    constexpr uintptr_t OFFSET_GROUND_SETCOLOR             = 0x12D380;

    // Initialize all hooks
    bool install();
    void remove();
    void applyLayoutMode(gd::PlayLayer* layer, bool enable);
    void applyLayoutModeColors(gd::PlayLayer* layer);
}
