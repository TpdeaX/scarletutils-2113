#include "hooks.hpp"
#include "includes.hpp"
#include "minhook/MinHook.h"
#include <climits>
#include <algorithm>

// Function pointers to original GD 2.113 functions
typedef bool (__thiscall* t_bgl_pushButton)(gd::GJBaseGameLayer*, int, bool);
typedef bool (__thiscall* t_bgl_releaseButton)(gd::GJBaseGameLayer*, int, bool);
typedef void (__thiscall* t_bgl_bumpPlayer)(gd::GJBaseGameLayer*, gd::PlayerObject*, gd::GameObject*);

typedef bool (__thiscall* t_playLayer_init)(gd::PlayLayer*, void*);
typedef void (__thiscall* t_playLayer_dtor)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_update)(gd::PlayLayer*, float);
typedef void (__thiscall* t_playLayer_addObject)(gd::PlayLayer*, gd::GameObject*);
typedef void (__thiscall* t_playLayer_destroyPlayer)(gd::PlayLayer*, gd::PlayerObject*, gd::GameObject*);
typedef void (__thiscall* t_playLayer_resetLevel)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_startGame)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_showEndLayer)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_showNewBest)(gd::PlayLayer*, bool, int, int, bool, bool, bool);
typedef void (__thiscall* t_playLayer_playGravityEffect)(gd::PlayLayer*, bool);
typedef void (__thiscall* t_playLayer_onEnterTransition)(gd::PlayLayer*);
typedef void (__thiscall* t_playLayer_toggleGlitter)(gd::PlayLayer*, bool);
typedef void (__thiscall* t_playLayer_updateColor)(gd::PlayLayer*, gd::ccColor3B, float, int, bool, float, gd::ccHSVValue, int, bool, int, gd::EffectGameObject*);

typedef void (__thiscall* t_player_spawnPortalCircle)(gd::PlayerObject*, gd::ccColor3B, float);
typedef void (__thiscall* t_player_playDeathEffect)(gd::PlayerObject*);
typedef void (__thiscall* t_player_stopDashing)(gd::PlayerObject*);
typedef void (__thiscall* t_player_propellPlayer)(gd::PlayerObject*, float);

typedef void (__thiscall* t_gameObj_addGlow)(gd::GameObject*);
typedef void (__thiscall* t_gameObj_playShineEffect)(gd::GameObject*);

typedef void (__thiscall* t_ringObj_spawnCircle)(gd::RingObject*);
typedef void (__thiscall* t_circleWave_draw)(gd::CCCircleWave*);

typedef void (__thiscall* t_pauseLayer_customSetup)(gd::PauseLayer*);
typedef void (__thiscall* t_endLevel_customSetup)(gd::EndLevelLayer*);

static t_bgl_pushButton         orig_bgl_pushButton         = nullptr;
static t_bgl_releaseButton      orig_bgl_releaseButton      = nullptr;
static t_bgl_bumpPlayer         orig_bgl_bumpPlayer         = nullptr;

static t_playLayer_init         orig_playLayer_init         = nullptr;
static t_playLayer_dtor         orig_playLayer_dtor         = nullptr;
static t_playLayer_update       orig_playLayer_update       = nullptr;
static t_playLayer_addObject    orig_playLayer_addObject    = nullptr;
static t_playLayer_destroyPlayer orig_playLayer_destroyPlayer = nullptr;
static t_playLayer_resetLevel   orig_playLayer_resetLevel   = nullptr;
static t_playLayer_startGame    orig_playLayer_startGame    = nullptr;
static t_playLayer_showEndLayer orig_playLayer_showEndLayer = nullptr;
static t_playLayer_showNewBest  orig_playLayer_showNewBest  = nullptr;
static t_playLayer_playGravityEffect orig_playLayer_playGravityEffect = nullptr;
static t_playLayer_onEnterTransition orig_playLayer_onEnterTransition = nullptr;
static t_playLayer_toggleGlitter orig_playLayer_toggleGlitter = nullptr;
static t_playLayer_updateColor  orig_playLayer_updateColor  = nullptr;

static t_player_spawnPortalCircle orig_player_spawnPortalCircle = nullptr;
static t_player_playDeathEffect   orig_player_playDeathEffect   = nullptr;
static t_player_stopDashing       orig_player_stopDashing       = nullptr;
static t_player_propellPlayer     orig_player_propellPlayer     = nullptr;

static t_gameObj_addGlow          orig_gameObj_addGlow          = nullptr;
static t_gameObj_playShineEffect  orig_gameObj_playShineEffect  = nullptr;

static t_ringObj_spawnCircle      orig_ringObj_spawnCircle      = nullptr;
static t_circleWave_draw          orig_circleWave_draw          = nullptr;

static t_pauseLayer_customSetup   orig_pauseLayer_customSetup   = nullptr;
static t_endLevel_customSetup     orig_endLevel_customSetup     = nullptr;

// Helper to maintain gravity holding state
static void runMaintainGravity(gd::PlayLayer* layer) {
    if (!maintainGravity || !layer || !layer->m_pPlayer1) return;

    gd::PlayerObject* p1 = layer->m_pPlayer1;
    bool p1Holding = p1->m_isHolding;
    bool p1Maintain = p1Holding != p1->m_isUpsideDown;

    if (p1Maintain != p1Holding) {
        if (p1Holding) orig_bgl_releaseButton(layer, 0, false);
        else orig_bgl_pushButton(layer, 0, false);
        autoclickerTimerP1 = INT_MAX;
    }

    if (layer->m_bIsDualMode && layer->m_pPlayer2) {
        gd::PlayerObject* p2 = layer->m_pPlayer2;
        bool p2Holding = p2->m_isHolding;
        bool p2Maintain = p2Holding != p2->m_isUpsideDown;
        if (p2Maintain != p2Holding) {
            if (p2Holding) orig_bgl_releaseButton(layer, 0, true);
            else orig_bgl_pushButton(layer, 0, true);
            autoclickerTimerP2 = INT_MAX;
        }
    }
}

// Hook: GJBaseGameLayer::pushButton
static bool __fastcall hook_bgl_pushButton(gd::GJBaseGameLayer* self, void* edx, int button, bool isPlayer2) {
    bool ret = orig_bgl_pushButton(self, button, isPlayer2);

    if (autoSwift) {
        orig_bgl_releaseButton(self, button, isPlayer2);
    }
    if (extraClick) {
        for (int i = 0; i < extraClickAmount; i++) {
            orig_bgl_releaseButton(self, button, isPlayer2);
            orig_bgl_pushButton(self, button, isPlayer2);
        }
    }
    return ret;
}

// Hook: GJBaseGameLayer::bumpPlayer
static void __fastcall hook_bgl_bumpPlayer(gd::GJBaseGameLayer* self, void* edx, gd::PlayerObject* player, gd::GameObject* object) {
    if (object) {
        int type = object->m_nObjectType;
        bool isP2 = (self->m_pPlayer2 && player == self->m_pPlayer2);

        // Jump pads: Yellow (8), Pink (9), Red (34)
        if (type == (int)gd::GameObjectType::YellowJumpPad ||
            type == (int)gd::GameObjectType::PinkJumpPad ||
            type == (int)gd::GameObjectType::RedJumpPad) {
            if (!clickedJumpPad && clickJumpPads) {
                orig_bgl_pushButton(self, 0, isP2);
                orig_bgl_releaseButton(self, 0, isP2);
                clickedJumpPad = true;
            }
        }
        // Gravity pad: Blue (10)
        else if (type == (int)gd::GameObjectType::GravityPad) {
            if (!clickedJumpPad && clickGravityPads) {
                orig_bgl_pushButton(self, 0, isP2);
                orig_bgl_releaseButton(self, 0, isP2);
                clickedJumpPad = true;
            }
        }
    }
    orig_bgl_bumpPlayer(self, player, object);
}

// Hook: PlayLayer::init
static bool __fastcall hook_playLayer_init(gd::PlayLayer* self, void* edx, void* level) {
    g_currentPlayLayer = self;
    return orig_playLayer_init(self, level);
}

// Hook: PlayLayer::~PlayLayer
static void __fastcall hook_playLayer_dtor(gd::PlayLayer* self, void* edx) {
    if (g_currentPlayLayer == self) {
        g_currentPlayLayer = nullptr;
    }
    orig_playLayer_dtor(self);
}

// Hook: PlayLayer::addObject
static void __fastcall hook_playLayer_addObject(gd::PlayLayer* self, void* edx, gd::GameObject* object) {
    if (!object) {
        orig_playLayer_addObject(self, object);
        return;
    }

    if (noEffect &&
        object->m_nObjectType != (int)gd::GameObjectType::InverseMirrorPortal &&
        object->m_nObjectType != (int)gd::GameObjectType::NormalMirrorPortal) {
        object->m_bHasNoEffects = true;
    }

    if (!layoutMode) {
        orig_playLayer_addObject(self, object);
        return;
    }

    if (filter.count(object->m_nObjectID)) {
        object->m_bIsHidden = true;
        return;
    }

    object->m_bHasNoGlow = true;
    object->m_bIsHidden = false;
    object->setOpacity(255);
    if (!noEffect) {
        object->m_bHasNoEffects = false;
    }

    orig_playLayer_addObject(self, object);
}

// Hook: PlayLayer::destroyPlayer
static void __fastcall hook_playLayer_destroyPlayer(gd::PlayLayer* self, void* edx, gd::PlayerObject* player, gd::GameObject* object) {
    if (!player || player->m_isDead) {
        orig_playLayer_destroyPlayer(self, player, object);
        return;
    }

    // Noclip checks
    if (noclip) {
        if (noclipP1 && player == self->m_pPlayer1) return;
        if (noclipP2 && player == self->m_pPlayer2) return;
    }

    orig_playLayer_destroyPlayer(self, player, object);

    flipPlayer = (player == self->m_pPlayer2) ? 2 : 1;

    autoclickerHoldingP1 = false;
    autoclickerTimerP1 = INT_MAX;
    autoclickerHoldingP2 = false;
    autoclickerTimerP2 = INT_MAX;
}

// Hook: PlayLayer::resetLevel
static void __fastcall hook_playLayer_resetLevel(gd::PlayLayer* self, void* edx) {
    orig_playLayer_resetLevel(self);

    if (flipOnDeath && flipPlayer != 0) {
        if ((flipOnDeathP1 && flipPlayer == 1) || flipOnDeathBoth) {
            if (!flipOnDeathSwift) {
                if (flipOnDeathLogicP1) {
                    orig_bgl_pushButton(self, 0, false);
                }
                flipPlayer = 0;
                flipOnDeathLogicP1 = !flipOnDeathLogicP1;
            } else {
                orig_bgl_pushButton(self, 0, false);
                orig_bgl_releaseButton(self, 0, false);
                flipPlayer = 0;
            }
        }
        if ((flipOnDeathP2 && flipPlayer == 2) || flipOnDeathBoth) {
            if (!flipOnDeathSwift) {
                if (flipOnDeathLogicP2) {
                    orig_bgl_pushButton(self, 0, true);
                }
                flipPlayer = 0;
                flipOnDeathLogicP2 = !flipOnDeathLogicP2;
            } else {
                orig_bgl_pushButton(self, 0, true);
                orig_bgl_releaseButton(self, 0, true);
                flipPlayer = 0;
            }
        }
        if (flipOnDeathUnfreeze && hwnd) {
            PostMessage(hwnd, WM_KEYDOWN, 0x56, 0);
            PostMessage(hwnd, WM_KEYUP, 0x56, 0);
        }
    }
}

// Hook: PlayLayer::startGame
static void __fastcall hook_playLayer_startGame(gd::PlayLayer* self, void* edx) {
    orig_playLayer_startGame(self);

    if (restartFirstFrame) {
        orig_playLayer_resetLevel(self);
    }
}

// Hook: PlayLayer::showEndLayer
static void __fastcall hook_playLayer_showEndLayer(gd::PlayLayer* self, void* edx) {
    if (!hideEndscreen) {
        orig_playLayer_showEndLayer(self);
    }
}

// Hook: PlayLayer::showNewBest
static void __fastcall hook_playLayer_showNewBest(gd::PlayLayer* self, void* edx, bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
    if (!hideNewBest) {
        orig_playLayer_showNewBest(self, newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    }
}

// Hook: PlayLayer::playGravityEffect
static void __fastcall hook_playLayer_playGravityEffect(gd::PlayLayer* self, void* edx, bool flip) {
    if (!noEffect) {
        orig_playLayer_playGravityEffect(self, flip);
    }
}

// Hook: PlayLayer::onEnterTransitionDidFinish
static void __fastcall hook_playLayer_onEnterTransition(gd::PlayLayer* self, void* edx) {
    if (layoutMode && orig_playLayer_toggleGlitter) {
        orig_playLayer_toggleGlitter(self, false);
    }
    orig_playLayer_onEnterTransition(self);
}

// Hook: PlayLayer::updateColor
static void __fastcall hook_playLayer_updateColor(
    gd::PlayLayer* self, void* edx,
    gd::ccColor3B color, float fadeTime, int colorID, bool blending,
    float opacity, gd::ccHSVValue copyHSV, int colorIDToCopy,
    bool copyOpacity, int unk, gd::EffectGameObject* caller) {

    if (layoutMode) {
        if (colorID == 1000) { // BG
            color = layoutModeColorBackground;
        } else if (colorID == 1001) { // G1
            color = layoutModeColorGround;
        } else {
            color = { 255, 255, 255 };
        }
    }

    orig_playLayer_updateColor(self, color, fadeTime, colorID, blending, opacity, copyHSV, colorIDToCopy, copyOpacity, unk, caller);
}

// Hook: PlayerObject::spawnPortalCircle
static void __fastcall hook_player_spawnPortalCircle(gd::PlayerObject* self, void* edx, gd::ccColor3B color, float startRadius) {
    if (!noEffect) {
        orig_player_spawnPortalCircle(self, color, startRadius);
    }
}

// Hook: PlayerObject::playDeathEffect
static void __fastcall hook_player_playDeathEffect(gd::PlayerObject* self, void* edx) {
    if (!noDeathEffect) {
        orig_player_playDeathEffect(self);
    }
}

// Hook: PlayerObject::stopDashing
static void __fastcall hook_player_stopDashing(gd::PlayerObject* self, void* edx) {
    if (noEffect || (optimizeStackedOrbs && optimizeStopDashing)) {
        return;
    }
    orig_player_stopDashing(self);
    optimizeStopDashing = true;
}

// Hook: GameObject::addGlow
static void __fastcall hook_gameObj_addGlow(gd::GameObject* self, void* edx) {
    orig_gameObj_addGlow(self);

    if (!g_currentPlayLayer || !layoutMode) return;

    self->m_bIsHidden = ((self->m_nObjectType == (int)gd::GameObjectType::Decoration &&
        self->m_nObjectID != 44 && self->m_nObjectID != 38 &&
        self->m_nObjectID != 749 && self->m_nObjectID != 747) ||
        self->m_bIsNoTouch || decoration.count(self->m_nObjectID));
}

// Hook: GameObject::playShineEffect
static void __fastcall hook_gameObj_playShineEffect(gd::GameObject* self, void* edx) {
    if (!noEffect) {
        orig_gameObj_playShineEffect(self);
    }
}

// Hook: RingObject::spawnCircle
static void __fastcall hook_ringObj_spawnCircle(gd::RingObject* self, void* edx) {
    static int lastOrb = 0;
    if (!noEffect || !(optimizeStackedOrbs && optimizeRingJump && lastOrb == self->m_nObjectID)) {
        orig_ringObj_spawnCircle(self);
    }
    optimizeRingJump = true;
    lastOrb = self->m_nObjectID;
}

// Hook: CCCircleWave::draw
static void __fastcall hook_circleWave_draw(gd::CCCircleWave* self, void* edx) {
    if (!noEffect) {
        orig_circleWave_draw(self);
    }
}

// Hook: PauseLayer::customSetup
static void __fastcall hook_pauseLayer_customSetup(gd::PauseLayer* self, void* edx) {
    orig_pauseLayer_customSetup(self);
    // Button setup can be integrated with Cocos2d menu if needed
}

// Hook: EndLevelLayer::customSetup
static void __fastcall hook_endLevel_customSetup(gd::EndLevelLayer* self, void* edx) {
    orig_endLevel_customSetup(self);
    if (hideEndscreen) {
        self->setVisible(false);
    }
}

// Main Frame Update Loop: PlayLayer::update
static void __fastcall hook_playLayer_update(gd::PlayLayer* self, void* edx, float dt) {
    clickedJumpPad = false;
    optimizeRingJump = false;
    optimizeStartDashing = false;
    optimizeStopDashing = false;

    if (layoutMode && orig_playLayer_toggleGlitter) {
        orig_playLayer_toggleGlitter(self, false);
    }

    gd::PlayerObject* p1 = self->m_pPlayer1;
    gd::PlayerObject* p2 = self->m_pPlayer2;

    // 1. Auto Straight UFO
    if (straightUfo && p1) {
        if (straightUfoP1) {
            float y = p1->getPositionY();
            double vel = p1->m_yVelocity;
            bool upsideDown = p1->m_isUpsideDown;
            if ((y < straightUfoTargetP1 - straightUfoThresholdP1 && vel <= 0 && !upsideDown) ||
                (y > straightUfoTargetP1 + straightUfoThresholdP1 && vel > 0 && !upsideDown) ||
                (y > straightUfoTargetP1 + straightUfoThresholdP1 && vel >= 0 && upsideDown) ||
                (y < straightUfoTargetP1 - straightUfoThresholdP1 && vel < 0 && upsideDown)) {
                orig_bgl_releaseButton(self, 0, false);
                orig_bgl_pushButton(self, 0, false);
            }
        }
        if (straightUfoP2 && p2 && self->m_bIsDualMode) {
            float y2 = p2->getPositionY();
            double vel2 = p2->m_yVelocity;
            bool upsideDown2 = p2->m_isUpsideDown;
            if ((y2 < straightUfoTargetP2 - straightUfoThresholdP2 && vel2 <= 0 && !upsideDown2) ||
                (y2 > straightUfoTargetP2 + straightUfoThresholdP2 && vel2 > 0 && !upsideDown2) ||
                (y2 > straightUfoTargetP2 + straightUfoThresholdP2 && vel2 >= 0 && upsideDown2) ||
                (y2 < straightUfoTargetP2 - straightUfoThresholdP2 && vel2 < 0 && upsideDown2)) {
                orig_bgl_releaseButton(self, 0, true);
                orig_bgl_pushButton(self, 0, true);
            }
        }
    }

    // 2. Auto Straight Fly
    if (straightFly && p1) {
        double vel = p1->m_yVelocity;
        bool upsideDown = p1->m_isUpsideDown;
        bool holding = p1->m_isHolding;
        if (straightFlyP1) {
            if ((vel < -straightFlyThresholdP1 && !holding && !upsideDown) ||
                (vel > straightFlyThresholdP1 && holding && !upsideDown) ||
                (vel > straightFlyThresholdP1 && !holding && upsideDown) ||
                (vel < -straightFlyThresholdP1 && holding && upsideDown)) {
                if (!holding) orig_bgl_pushButton(self, 0, false);
                else orig_bgl_releaseButton(self, 0, false);
            }
        }
        if (straightFlyP2 && p2 && self->m_bIsDualMode) {
            double vel2 = p2->m_yVelocity;
            bool upsideDown2 = p2->m_isUpsideDown;
            bool holding2 = p2->m_isHolding;
            if ((vel2 < -straightFlyThresholdP2 && !holding2 && !upsideDown2) ||
                (vel2 > straightFlyThresholdP2 && holding2 && !upsideDown2) ||
                (vel2 > straightFlyThresholdP2 && !holding2 && upsideDown2) ||
                (vel2 < -straightFlyThresholdP2 && holding2 && upsideDown2)) {
                if (!holding2) orig_bgl_pushButton(self, 0, true);
                else orig_bgl_releaseButton(self, 0, true);
            }
        }
    }

    // 3. Autoclicker P1
    if (autoclickerP1) {
        if (autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerHoldP1) {
            orig_bgl_releaseButton(self, 0, false);
            autoclickerHoldingP1 = false;
            autoclickerTimerP1 = 0;
        } else if (!autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerEveryP1) {
            orig_bgl_pushButton(self, 0, false);
            if (autoclickerSwiftP1) {
                orig_bgl_releaseButton(self, 0, false);
            }
            autoclickerHoldingP1 = true;
            autoclickerTimerP1 = 0;
        }
        autoclickerTimerP1++;
    }

    // 4. Autoclicker P2
    if (autoclickerP2 && self->m_bIsDualMode) {
        if (autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerHoldP2) {
            orig_bgl_releaseButton(self, 0, true);
            autoclickerHoldingP2 = false;
            autoclickerTimerP2 = 0;
        } else if (!autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerEveryP2) {
            orig_bgl_pushButton(self, 0, true);
            if (autoclickerSwiftP2) {
                orig_bgl_releaseButton(self, 0, true);
            }
            autoclickerHoldingP2 = true;
            autoclickerTimerP2 = 0;
        }
        autoclickerTimerP2++;
    }

    // 5. Touching Rings (Green dash, black orb, gravity orbs)
    if (p1 && p1->m_touchingRings) {
        int count = p1->m_touchingRings->count();
        for (int i = 0; i < count; i++) {
            auto obj = static_cast<gd::GameObject*>(p1->m_touchingRings->objectAtIndex(i));
            if (!obj) continue;

            if (obj->m_nObjectType == (int)gd::GameObjectType::DashRing && clickGreenDash) {
                orig_bgl_releaseButton(self, 0, false);
                orig_bgl_pushButton(self, 0, false);
            }
            if (obj->m_nObjectType == (int)gd::GameObjectType::DropRing && (blackOrbUfo || straightUfo || clickBlackOrbs)) {
                if ((p1->m_yVelocity <= 0 && !p1->m_isUpsideDown) ||
                    (p1->m_yVelocity >= 0 && p1->m_isUpsideDown) ||
                    clickBlackOrbs) {
                    orig_bgl_releaseButton(self, 0, false);
                    orig_bgl_pushButton(self, 0, false);
                }
            }
            if ((obj->m_nObjectType == (int)gd::GameObjectType::GravityDashRing ||
                 obj->m_nObjectType == (int)gd::GameObjectType::GravityRing ||
                 obj->m_nObjectType == (int)gd::GameObjectType::GreenRing) && maintainGravity) {
                orig_bgl_releaseButton(self, 0, false);
            }
        }
    }

    runMaintainGravity(self);

    orig_playLayer_update(self, dt);
}

namespace Hooks {

bool install() {
    MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        return false;
    }

    #define CREATE_HOOK(offset, detour, original) \
        do { \
            MH_STATUS s = MH_CreateHook(reinterpret_cast<LPVOID>(gd_base + (offset)), reinterpret_cast<LPVOID>(&detour), reinterpret_cast<LPVOID*>(&original)); \
            if (s == MH_OK || s == MH_ERROR_ALREADY_CREATED) { \
                MH_EnableHook(reinterpret_cast<LPVOID>(gd_base + (offset))); \
            } \
        } while (0)

    CREATE_HOOK(OFFSET_BGL_PUSH_BUTTON,         hook_bgl_pushButton,         orig_bgl_pushButton);
    CREATE_HOOK(OFFSET_BGL_BUMP_PLAYER,          hook_bgl_bumpPlayer,          orig_bgl_bumpPlayer);

    CREATE_HOOK(OFFSET_PLAYLAYER_INIT,          hook_playLayer_init,          orig_playLayer_init);
    CREATE_HOOK(OFFSET_PLAYLAYER_DTOR,          hook_playLayer_dtor,          orig_playLayer_dtor);
    CREATE_HOOK(OFFSET_PLAYLAYER_UPDATE,        hook_playLayer_update,        orig_playLayer_update);
    CREATE_HOOK(OFFSET_PLAYLAYER_ADDOBJECT,     hook_playLayer_addObject,     orig_playLayer_addObject);
    CREATE_HOOK(OFFSET_PLAYLAYER_DESTROYPLAYER, hook_playLayer_destroyPlayer, orig_playLayer_destroyPlayer);
    CREATE_HOOK(OFFSET_PLAYLAYER_RESETLEVEL,    hook_playLayer_resetLevel,    orig_playLayer_resetLevel);
    CREATE_HOOK(OFFSET_PLAYLAYER_STARTGAME,     hook_playLayer_startGame,     orig_playLayer_startGame);
    CREATE_HOOK(OFFSET_PLAYLAYER_SHOWENDLAYER,  hook_playLayer_showEndLayer,  orig_playLayer_showEndLayer);
    CREATE_HOOK(OFFSET_PLAYLAYER_SHOWNEWBEST,   hook_playLayer_showNewBest,   orig_playLayer_showNewBest);
    CREATE_HOOK(OFFSET_PLAYLAYER_GRAVITYEFF,    hook_playLayer_playGravityEffect, orig_playLayer_playGravityEffect);
    CREATE_HOOK(OFFSET_PLAYLAYER_TRANSITION,    hook_playLayer_onEnterTransition, orig_playLayer_onEnterTransition);
    CREATE_HOOK(OFFSET_PLAYLAYER_UPDATECOLOR,   hook_playLayer_updateColor,   orig_playLayer_updateColor);

    CREATE_HOOK(OFFSET_PLAYER_SPAWNPORTALCIRC,  hook_player_spawnPortalCircle, orig_player_spawnPortalCircle);
    CREATE_HOOK(OFFSET_PLAYER_PLAYDEATHEFF,     hook_player_playDeathEffect,   orig_player_playDeathEffect);
    CREATE_HOOK(OFFSET_PLAYER_STOPDASHING,      hook_player_stopDashing,       orig_player_stopDashing);

    CREATE_HOOK(OFFSET_GAMEOBJ_ADDGLOW,         hook_gameObj_addGlow,         orig_gameObj_addGlow);
    CREATE_HOOK(OFFSET_GAMEOBJ_PLAYSHINEEFF,    hook_gameObj_playShineEffect,  orig_gameObj_playShineEffect);

    CREATE_HOOK(OFFSET_RINGOBJ_SPAWNCIRCLE,     hook_ringObj_spawnCircle,     orig_ringObj_spawnCircle);
    CREATE_HOOK(OFFSET_CIRCLEWAVE_DRAW,         hook_circleWave_draw,         orig_circleWave_draw);

    CREATE_HOOK(OFFSET_PAUSELAYER_CUSTOMSETUP,  hook_pauseLayer_customSetup,  orig_pauseLayer_customSetup);
    CREATE_HOOK(OFFSET_ENDLEVEL_CUSTOMSETUP,    hook_endLevel_customSetup,    orig_endLevel_customSetup);

    // Resolve non-hooked helper addresses
    orig_bgl_releaseButton = reinterpret_cast<t_bgl_releaseButton>(gd_base + OFFSET_BGL_RELEASE_BUTTON);
    orig_playLayer_toggleGlitter = reinterpret_cast<t_playLayer_toggleGlitter>(gd_base + OFFSET_PLAYLAYER_TOGGLEGLITTER);

    #undef CREATE_HOOK

    return true;
}

void remove() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

} // namespace Hooks
