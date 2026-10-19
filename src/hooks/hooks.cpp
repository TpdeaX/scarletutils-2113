#include "hooks.hpp"
#include "includes.hpp"
#include "minhook/MinHook.h"
#include <climits>
#include <algorithm>
#include <cstdio>

// Logging helper for debugging
static void Log(const char* fmt, ...) {
    FILE* f = fopen("scarletutils.log", "a");
    if (!f) return;
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
}

// Function pointers to original GD 2.113 functions
typedef bool (__thiscall* t_bgl_pushButton)(gd::GJBaseGameLayer*, int, bool);
typedef bool (__thiscall* t_bgl_releaseButton)(gd::GJBaseGameLayer*, int, bool);
typedef void (__stdcall*  t_bgl_bumpPlayer)(gd::PlayerObject*, gd::GameObject*);

typedef bool (__thiscall* t_playLayer_init)(gd::PlayLayer*, void*);
typedef void (__thiscall* t_playLayer_dtor)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_update)(gd::PlayLayer*, float);
typedef void (__thiscall* t_playLayer_destroyPlayer)(gd::PlayLayer*, gd::PlayerObject*, gd::GameObject*);
typedef void (__thiscall* t_playLayer_resetLevel)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_startGame)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_showEndLayer)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_showNewBest)(gd::PlayLayer*, bool, int, int, bool, bool, bool);
typedef void (__thiscall* t_playLayer_playGravityEffect)(gd::PlayLayer*, bool);
typedef void (__thiscall* t_playLayer_onEnterTransition)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_toggleGlitter)(gd::PlayLayer*, bool);

typedef void (__thiscall* t_player_playDeathEffect)(gd::PlayerObject*);
typedef void (__thiscall* t_player_spawnPortalCircle)(gd::PlayerObject*, gd::ccColor3B, float);
typedef void (__thiscall* t_player_stopDashing)(gd::PlayerObject*);

typedef void (__thiscall* t_gameobj_playShineEffect)(gd::GameObject*);
typedef void (__thiscall* t_ring_spawnCircle)(gd::RingObject*);
typedef void (__thiscall* t_circleWave_draw)(void*);

static t_bgl_pushButton         orig_bgl_pushButton         = nullptr;
static t_bgl_releaseButton      orig_bgl_releaseButton      = nullptr;
static t_bgl_bumpPlayer         orig_bgl_bumpPlayer         = nullptr;

static t_playLayer_init         orig_playLayer_init         = nullptr;
static t_playLayer_dtor         orig_playLayer_dtor         = nullptr;
static t_playLayer_update       orig_playLayer_update       = nullptr;
static t_playLayer_destroyPlayer orig_playLayer_destroyPlayer = nullptr;
static t_playLayer_resetLevel   orig_playLayer_resetLevel   = nullptr;
static t_playLayer_startGame    orig_playLayer_startGame    = nullptr;
static t_playLayer_showEndLayer orig_playLayer_showEndLayer = nullptr;
static t_playLayer_showNewBest  orig_playLayer_showNewBest  = nullptr;
static t_playLayer_playGravityEffect orig_playLayer_playGravityEffect = nullptr;
static t_playLayer_onEnterTransition orig_playLayer_onEnterTransition = nullptr;
static t_playLayer_toggleGlitter orig_playLayer_toggleGlitter = nullptr;

static t_player_playDeathEffect orig_player_playDeathEffect = nullptr;
static t_player_spawnPortalCircle orig_player_spawnPortalCircle = nullptr;
static t_player_stopDashing     orig_player_stopDashing     = nullptr;

static t_gameobj_playShineEffect orig_gameobj_playShineEffect = nullptr;
static t_ring_spawnCircle       orig_ring_spawnCircle       = nullptr;
static t_circleWave_draw        orig_circleWave_draw        = nullptr;

// Helper to maintain gravity holding state
static void runMaintainGravity(gd::PlayLayer* layer) {
    __try {
        if (!maintainGravity || !layer || !layer->getPlayer1()) return;

        gd::PlayerObject* p1 = layer->getPlayer1();
        bool p1Holding = p1->isHolding();
        bool p1Maintain = p1Holding != p1->isUpsideDown();

        if (p1Maintain != p1Holding) {
            if (p1Holding && orig_bgl_releaseButton) orig_bgl_releaseButton(layer, 1, false);
            else if (orig_bgl_pushButton) orig_bgl_pushButton(layer, 1, false);
            autoclickerTimerP1 = INT_MAX;
        }

        if (layer->isDualMode() && layer->getPlayer2()) {
            gd::PlayerObject* p2 = layer->getPlayer2();
            bool p2Holding = p2->isHolding();
            bool p2Maintain = p2Holding != p2->isUpsideDown();
            if (p2Maintain != p2Holding) {
                if (p2Holding && orig_bgl_releaseButton) orig_bgl_releaseButton(layer, 1, true);
                else if (orig_bgl_pushButton) orig_bgl_pushButton(layer, 1, true);
                autoclickerTimerP2 = INT_MAX;
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
}

// Hook: GJBaseGameLayer::pushButton
static bool __fastcall hook_bgl_pushButton(gd::GJBaseGameLayer* self, void* edx, int button, bool isPlayer2) {
    bool ret = false;
    if (orig_bgl_pushButton) {
        ret = orig_bgl_pushButton(self, button, isPlayer2);
    }

    __try {
        if (autoSwift && orig_bgl_releaseButton) {
            orig_bgl_releaseButton(self, button, isPlayer2);
        }
        if (extraClick && orig_bgl_releaseButton && orig_bgl_pushButton) {
            for (int i = 0; i < extraClickAmount; i++) {
                orig_bgl_releaseButton(self, button, isPlayer2);
                orig_bgl_pushButton(self, button, isPlayer2);
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {}

    return ret;
}

// Hook: GJBaseGameLayer::bumpPlayer (__stdcall on GD 2.113 Win32)
static void __stdcall hook_bgl_bumpPlayer(gd::PlayerObject* player, gd::GameObject* object) {
    __try {
        if (object && player && g_currentPlayLayer) {
            int type = object->getObjectType();
            bool isP2 = (g_currentPlayLayer->getPlayer2() && player == g_currentPlayLayer->getPlayer2());

            // Jump pads: Yellow (8), Pink (9), Red (34)
            if (type == (int)gd::GameObjectType::YellowJumpPad ||
                type == (int)gd::GameObjectType::PinkJumpPad ||
                type == (int)gd::GameObjectType::RedJumpPad) {
                if (!clickedJumpPad && clickJumpPads && orig_bgl_pushButton && orig_bgl_releaseButton) {
                    orig_bgl_pushButton(g_currentPlayLayer, 1, isP2);
                    orig_bgl_releaseButton(g_currentPlayLayer, 1, isP2);
                    clickedJumpPad = true;
                }
            }
            // Gravity pad: Blue (10)
            else if (type == (int)gd::GameObjectType::GravityPad) {
                if (!clickedJumpPad && clickGravityPads && orig_bgl_pushButton && orig_bgl_releaseButton) {
                    orig_bgl_pushButton(g_currentPlayLayer, 1, isP2);
                    orig_bgl_releaseButton(g_currentPlayLayer, 1, isP2);
                    clickedJumpPad = true;
                }
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {}

    if (orig_bgl_bumpPlayer) {
        orig_bgl_bumpPlayer(player, object);
    }
}

// Hook: PlayLayer::init
static bool __fastcall hook_playLayer_init(gd::PlayLayer* self, void* edx, void* level) {
    g_currentPlayLayer = self;
    bool ret = true;
    if (orig_playLayer_init) {
        ret = orig_playLayer_init(self, level);
    }
    return ret;
}

// Hook: PlayLayer::~PlayLayer
static void __fastcall hook_playLayer_dtor(gd::PlayLayer* self, void* edx) {
    if (g_currentPlayLayer == self) {
        g_currentPlayLayer = nullptr;
    }
    if (orig_playLayer_dtor) {
        orig_playLayer_dtor(self);
    }
}

// Hook: PlayLayer::destroyPlayer
static void __fastcall hook_playLayer_destroyPlayer(gd::PlayLayer* self, void* edx, gd::PlayerObject* player, gd::GameObject* object) {
    __try {
        if (!player || player->isDead()) {
            if (orig_playLayer_destroyPlayer) orig_playLayer_destroyPlayer(self, player, object);
            return;
        }

        // Noclip checks
        if (noclip) {
            if (noclipP1 && self->getPlayer1() && player == self->getPlayer1()) return;
            if (noclipP2 && self->getPlayer2() && player == self->getPlayer2()) return;
        }

        flipPlayer = (self->getPlayer2() && player == self->getPlayer2()) ? 2 : 1;
        autoclickerHoldingP1 = false;
        autoclickerTimerP1 = INT_MAX;
        autoclickerHoldingP2 = false;
        autoclickerTimerP2 = INT_MAX;
    } __except (EXCEPTION_EXECUTE_HANDLER) {}

    if (orig_playLayer_destroyPlayer) {
        orig_playLayer_destroyPlayer(self, player, object);
    }
}

// Hook: PlayLayer::resetLevel
static void __fastcall hook_playLayer_resetLevel(gd::PlayLayer* self, void* edx) {
    if (orig_playLayer_resetLevel) {
        orig_playLayer_resetLevel(self);
    }

    __try {
        if (flipOnDeath && flipPlayer != 0) {
            if ((flipOnDeathP1 && flipPlayer == 1) || flipOnDeathBoth) {
                if (!flipOnDeathSwift) {
                    if (flipOnDeathLogicP1 && orig_bgl_pushButton) {
                        orig_bgl_pushButton(self, 1, false);
                    }
                    flipPlayer = 0;
                    flipOnDeathLogicP1 = !flipOnDeathLogicP1;
                } else if (orig_bgl_pushButton && orig_bgl_releaseButton) {
                    orig_bgl_pushButton(self, 1, false);
                    orig_bgl_releaseButton(self, 1, false);
                    flipPlayer = 0;
                }
            }
            if ((flipOnDeathP2 && flipPlayer == 2) || flipOnDeathBoth) {
                if (!flipOnDeathSwift) {
                    if (flipOnDeathLogicP2 && orig_bgl_pushButton) {
                        orig_bgl_pushButton(self, 1, true);
                    }
                    flipPlayer = 0;
                    flipOnDeathLogicP2 = !flipOnDeathLogicP2;
                } else if (orig_bgl_pushButton && orig_bgl_releaseButton) {
                    orig_bgl_pushButton(self, 1, true);
                    orig_bgl_releaseButton(self, 1, true);
                    flipPlayer = 0;
                }
            }
            if (flipOnDeathUnfreeze && hwnd) {
                PostMessageA(hwnd, WM_KEYDOWN, 0x56, 0);
                PostMessageA(hwnd, WM_KEYUP, 0x56, 0);
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
}

// Hook: PlayLayer::startGame
static void __fastcall hook_playLayer_startGame(gd::PlayLayer* self, void* edx) {
    if (orig_playLayer_startGame) {
        orig_playLayer_startGame(self);
    }

    __try {
        if (restartFirstFrame && orig_playLayer_resetLevel) {
            orig_playLayer_resetLevel(self);
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
}

// Hook: PlayLayer::showEndLayer
static void __fastcall hook_playLayer_showEndLayer(gd::PlayLayer* self, void* edx) {
    if (!hideEndscreen && orig_playLayer_showEndLayer) {
        orig_playLayer_showEndLayer(self);
    }
}

// Hook: PlayLayer::showNewBest
static void __fastcall hook_playLayer_showNewBest(gd::PlayLayer* self, void* edx, bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
    if (!hideNewBest && orig_playLayer_showNewBest) {
        orig_playLayer_showNewBest(self, newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    }
}

// Hook: PlayLayer::playGravityEffect
static void __fastcall hook_playLayer_playGravityEffect(gd::PlayLayer* self, void* edx, bool flip) {
    if (!noEffect && orig_playLayer_playGravityEffect) {
        orig_playLayer_playGravityEffect(self, flip);
    }
}

// Hook: PlayLayer::onEnterTransitionDidFinish
static void __fastcall hook_playLayer_onEnterTransition(gd::PlayLayer* self, void* edx) {
    if (layoutMode && orig_playLayer_toggleGlitter) {
        orig_playLayer_toggleGlitter(self, false);
    }
    if (orig_playLayer_onEnterTransition) {
        orig_playLayer_onEnterTransition(self);
    }
}

// Hook: PlayerObject::playDeathEffect
static void __fastcall hook_player_playDeathEffect(gd::PlayerObject* self, void* edx) {
    if (!noDeathEffect && orig_player_playDeathEffect) {
        orig_player_playDeathEffect(self);
    }
}

// Hook: PlayerObject::spawnPortalCircle
static void __fastcall hook_player_spawnPortalCircle(gd::PlayerObject* self, void* edx, gd::ccColor3B color, float radius) {
    if (!noEffect && orig_player_spawnPortalCircle) {
        orig_player_spawnPortalCircle(self, color, radius);
    }
}

// Hook: PlayerObject::stopDashing
static void __fastcall hook_player_stopDashing(gd::PlayerObject* self, void* edx) {
    if (orig_player_stopDashing) {
        orig_player_stopDashing(self);
    }
    optimizeStopDashing = true;
}

// Hook: RingObject::spawnCircle
static void __fastcall hook_ring_spawnCircle(gd::RingObject* self, void* edx) {
    static int lastOrb = 0;
    int objID = self->getObjectID();
    if (!noEffect || !(optimizeStackedOrbs && optimizeRingJump && lastOrb == objID)) {
        if (orig_ring_spawnCircle) orig_ring_spawnCircle(self);
    }
    optimizeRingJump = true;
    lastOrb = objID;
}

// Hook: GameObject::playShineEffect
static void __fastcall hook_gameobj_playShineEffect(gd::GameObject* self, void* edx) {
    if (!noEffect && orig_gameobj_playShineEffect) {
        orig_gameobj_playShineEffect(self);
    }
}

// Hook: CCCircleWave::draw
static void __fastcall hook_circleWave_draw(void* self, void* edx) {
    if (!noEffect && orig_circleWave_draw) {
        orig_circleWave_draw(self);
    }
}

// Main Frame Update Loop: PlayLayer::update
static void __fastcall hook_playLayer_update(gd::PlayLayer* self, void* edx, float dt) {
    __try {
        clickedJumpPad = false;
        optimizeRingJump = false;
        optimizeStartDashing = false;
        optimizeStopDashing = false;

        if (layoutMode && orig_playLayer_toggleGlitter) {
            orig_playLayer_toggleGlitter(self, false);
        }

        gd::PlayerObject* p1 = self->getPlayer1();
        gd::PlayerObject* p2 = self->getPlayer2();

        // 0. Rings & Orbs handling
        auto processRings = [&](gd::PlayerObject* player, bool isP2) {
            if (!player) return;
            gd::CCArray* rings = player->getTouchingRings();
            if (!rings) return;
            unsigned int count = rings->count();
            for (unsigned int i = 0; i < count; i++) {
                auto obj = static_cast<gd::GameObject*>(rings->objectAtIndex(i));
                if (!obj) continue;
                int objType = obj->getObjectType();

                // Green Dash Ring (37)
                if (objType == (int)gd::GameObjectType::DashRing && clickGreenDash) {
                    if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, isP2);
                    if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, isP2);
                }

                // Black Orb / Drop Ring (32)
                if (objType == (int)gd::GameObjectType::DropRing && (clickBlackOrbs || blackOrbUfo || straightUfo)) {
                    double vel = player->getYVelocity();
                    bool upsideDown = player->isUpsideDown();
                    if (clickBlackOrbs || (!upsideDown && vel <= 0.0) || (upsideDown && vel >= 0.0)) {
                        if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, isP2);
                        if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, isP2);
                    }
                }

                // Gravity Orbs (13, 29, 38)
                if ((objType == (int)gd::GameObjectType::GravityRing ||
                     objType == (int)gd::GameObjectType::GreenRing ||
                     objType == (int)gd::GameObjectType::GravityDashRing) && maintainGravity) {
                    if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, isP2);
                }
            }
        };

        processRings(p1, false);
        if (self->isDualMode() && p2) {
            processRings(p2, true);
        }

        // 1. Auto Straight UFO
        if (straightUfo && p1) {
            if (straightUfoP1) {
                float y = p1->getPositionY();
                double vel = p1->getYVelocity();
                bool upsideDown = p1->isUpsideDown();
                if ((y < straightUfoTargetP1 - straightUfoThresholdP1 && vel <= 0 && !upsideDown) ||
                    (y > straightUfoTargetP1 + straightUfoThresholdP1 && vel > 0 && !upsideDown) ||
                    (y > straightUfoTargetP1 + straightUfoThresholdP1 && vel >= 0 && upsideDown) ||
                    (y < straightUfoTargetP1 - straightUfoThresholdP1 && vel < 0 && upsideDown)) {
                    if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, false);
                    if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, false);
                }
            }
            if (straightUfoP2 && p2 && self->isDualMode()) {
                float y2 = p2->getPositionY();
                double vel2 = p2->getYVelocity();
                bool upsideDown2 = p2->isUpsideDown();
                if ((y2 < straightUfoTargetP2 - straightUfoThresholdP2 && vel2 <= 0 && !upsideDown2) ||
                    (y2 > straightUfoTargetP2 + straightUfoThresholdP2 && vel2 > 0 && !upsideDown2) ||
                    (y2 > straightUfoTargetP2 + straightUfoThresholdP2 && vel2 >= 0 && upsideDown2) ||
                    (y2 < straightUfoTargetP2 - straightUfoThresholdP2 && vel2 < 0 && upsideDown2)) {
                    if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, true);
                    if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, true);
                }
            }
        }

        // 2. Auto Straight Fly
        if (straightFly && p1) {
            double vel = p1->getYVelocity();
            bool upsideDown = p1->isUpsideDown();
            bool holding = p1->isHolding();
            if (straightFlyP1) {
                if ((vel < -straightFlyThresholdP1 && !holding && !upsideDown) ||
                    (vel > straightFlyThresholdP1 && holding && !upsideDown) ||
                    (vel > straightFlyThresholdP1 && !holding && upsideDown) ||
                    (vel < -straightFlyThresholdP1 && holding && upsideDown)) {
                    if (!holding && orig_bgl_pushButton) orig_bgl_pushButton(self, 1, false);
                    else if (holding && orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, false);
                }
            }
            if (straightFlyP2 && p2 && self->isDualMode()) {
                double vel2 = p2->getYVelocity();
                bool upsideDown2 = p2->isUpsideDown();
                bool holding2 = p2->isHolding();
                if ((vel2 < -straightFlyThresholdP2 && !holding2 && !upsideDown2) ||
                    (vel2 > straightFlyThresholdP2 && holding2 && !upsideDown2) ||
                    (vel2 > straightFlyThresholdP2 && !holding2 && upsideDown2) ||
                    (vel2 < -straightFlyThresholdP2 && holding2 && upsideDown2)) {
                    if (!holding2 && orig_bgl_pushButton) orig_bgl_pushButton(self, 1, true);
                    else if (holding2 && orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, true);
                }
            }
        }

        // 3. Autoclicker P1
        if (autoclickerP1) {
            if (autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerHoldP1) {
                if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, false);
                autoclickerHoldingP1 = false;
                autoclickerTimerP1 = 0;
            } else if (!autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerEveryP1) {
                if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, false);
                if (autoclickerSwiftP1 && orig_bgl_releaseButton) {
                    orig_bgl_releaseButton(self, 1, false);
                }
                autoclickerHoldingP1 = true;
                autoclickerTimerP1 = 0;
            }
            autoclickerTimerP1++;
        }

        // 4. Autoclicker P2
        if (autoclickerP2 && self->isDualMode()) {
            if (autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerHoldP2) {
                if (orig_bgl_releaseButton) orig_bgl_releaseButton(self, 1, true);
                autoclickerHoldingP2 = false;
                autoclickerTimerP2 = 0;
            } else if (!autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerEveryP2) {
                if (orig_bgl_pushButton) orig_bgl_pushButton(self, 1, true);
                if (autoclickerSwiftP2 && orig_bgl_releaseButton) {
                    orig_bgl_releaseButton(self, 1, true);
                }
                autoclickerHoldingP2 = true;
                autoclickerTimerP2 = 0;
            }
            autoclickerTimerP2++;
        }

        runMaintainGravity(self);
    } __except (EXCEPTION_EXECUTE_HANDLER) {}

    if (orig_playLayer_update) {
        orig_playLayer_update(self, dt);
    }
}

namespace Hooks {

bool install() {
    Log("[ScarletUtils] Hooks::install() starting...");

    MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("[ScarletUtils] MH_Initialize failed with code: %d", initStatus);
        return false;
    }

    #define CREATE_HOOK(name, offset, detour, original) \
        do { \
            MH_STATUS s = MH_CreateHook(reinterpret_cast<LPVOID>(gd_base + (offset)), reinterpret_cast<LPVOID>(&detour), reinterpret_cast<LPVOID*>(&original)); \
            if (s == MH_OK || s == MH_ERROR_ALREADY_CREATED) { \
                MH_STATUS es = MH_EnableHook(reinterpret_cast<LPVOID>(gd_base + (offset))); \
                Log("[ScarletUtils] Hook " name " enabled (status: %d)", es); \
            } else { \
                Log("[ScarletUtils] Hook " name " FAILED to create: %d", s); \
            } \
        } while (0)

    CREATE_HOOK("bgl_pushButton",         OFFSET_BGL_PUSH_BUTTON,         hook_bgl_pushButton,         orig_bgl_pushButton);
    CREATE_HOOK("bgl_bumpPlayer",          OFFSET_BGL_BUMP_PLAYER,          hook_bgl_bumpPlayer,          orig_bgl_bumpPlayer);

    CREATE_HOOK("playLayer_init",          OFFSET_PLAYLAYER_INIT,          hook_playLayer_init,          orig_playLayer_init);
    CREATE_HOOK("playLayer_dtor",          OFFSET_PLAYLAYER_DTOR,          hook_playLayer_dtor,          orig_playLayer_dtor);
    CREATE_HOOK("playLayer_update",        OFFSET_PLAYLAYER_UPDATE,        hook_playLayer_update,        orig_playLayer_update);
    CREATE_HOOK("playLayer_destroyPlayer",  OFFSET_PLAYLAYER_DESTROYPLAYER, hook_playLayer_destroyPlayer, orig_playLayer_destroyPlayer);
    CREATE_HOOK("playLayer_resetLevel",    OFFSET_PLAYLAYER_RESETLEVEL,    hook_playLayer_resetLevel,    orig_playLayer_resetLevel);
    CREATE_HOOK("playLayer_startGame",     OFFSET_PLAYLAYER_STARTGAME,     hook_playLayer_startGame,     orig_playLayer_startGame);
    CREATE_HOOK("playLayer_showEndLayer",  OFFSET_PLAYLAYER_SHOWENDLAYER,  hook_playLayer_showEndLayer,  orig_playLayer_showEndLayer);
    CREATE_HOOK("playLayer_showNewBest",   OFFSET_PLAYLAYER_SHOWNEWBEST,   hook_playLayer_showNewBest,   orig_playLayer_showNewBest);
    CREATE_HOOK("playLayer_gravityEff",    OFFSET_PLAYLAYER_GRAVITYEFF,    hook_playLayer_playGravityEffect, orig_playLayer_playGravityEffect);
    CREATE_HOOK("playLayer_transition",    OFFSET_PLAYLAYER_TRANSITION,    hook_playLayer_onEnterTransition, orig_playLayer_onEnterTransition);

    CREATE_HOOK("player_playDeathEffect",  OFFSET_PLAYER_PLAYDEATHEFF,     hook_player_playDeathEffect,   orig_player_playDeathEffect);
    CREATE_HOOK("player_spawnPortalCircle",OFFSET_PLAYER_SPAWNPORTALCIRC,  hook_player_spawnPortalCircle, orig_player_spawnPortalCircle);
    CREATE_HOOK("player_stopDashing",      OFFSET_PLAYER_STOPDASHING,      hook_player_stopDashing,       orig_player_stopDashing);

    CREATE_HOOK("gameobj_playShineEff",    OFFSET_GAMEOBJ_PLAYSHINEEFF,    hook_gameobj_playShineEffect,  orig_gameobj_playShineEffect);
    CREATE_HOOK("ring_spawnCircle",        OFFSET_RINGOBJ_SPAWNCIRCLE,     hook_ring_spawnCircle,         orig_ring_spawnCircle);
    CREATE_HOOK("circleWave_draw",         OFFSET_CIRCLEWAVE_DRAW,         hook_circleWave_draw,          orig_circleWave_draw);

    // Resolve non-hooked helper addresses
    orig_bgl_releaseButton = reinterpret_cast<t_bgl_releaseButton>(gd_base + OFFSET_BGL_RELEASE_BUTTON);
    orig_playLayer_toggleGlitter = reinterpret_cast<t_playLayer_toggleGlitter>(gd_base + OFFSET_PLAYLAYER_TOGGLEGLITTER);

    #undef CREATE_HOOK

    Log("[ScarletUtils] All hooks registered successfully!");
    return true;
}

void remove() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

} // namespace Hooks
