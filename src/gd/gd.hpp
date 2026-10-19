#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>

namespace gd {

// Common types & vectors
struct CCPoint {
    float x;
    float y;
    CCPoint() : x(0.0f), y(0.0f) {}
    CCPoint(float _x, float _y) : x(_x), y(_y) {}
};

struct CCSize {
    float width;
    float height;
    CCSize() : width(0.0f), height(0.0f) {}
    CCSize(float _w, float _h) : width(_w), height(_h) {}
};

struct CCRect {
    CCPoint origin;
    CCSize size;
    CCRect() : origin(), size() {}
    CCRect(float x, float y, float w, float h) : origin(x, y), size(w, h) {}
};

struct ccColor3B {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ccColor4B {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct ccHSVValue {
    float h;
    float s;
    float v;
    bool absoluteSaturation;
    bool absoluteBrightness;
};

// Forward declarations
class CCObject;
class CCNode;
class CCArray;
class CCDictionary;
class CCSprite;
class PlayerObject;
class GameObject;
class GJBaseGameLayer;
class PlayLayer;
class LevelSettingsObject;
class CheckpointObject;

// Cocos2d-x minimal classes
class CCObject {
public:
    virtual ~CCObject() {}
    int m_uReference;
    int m_uAutoReleaseCount;
};

class CCArray : public CCObject {
public:
    struct cc_array {
        unsigned int num;
        unsigned int max;
        CCObject** arr;
    };

    cc_array* getArrayData() const {
        return *reinterpret_cast<cc_array* const*>(uintptr_t(this) + 0x20);
    }

    unsigned int count() const {
        auto d = getArrayData();
        return d ? d->num : 0;
    }

    CCObject* objectAtIndex(unsigned int index) const {
        auto d = getArrayData();
        if (!d || !d->arr || index >= d->num) return nullptr;
        return d->arr[index];
    }
};

class CCNode : public CCObject {
public:
    virtual ~CCNode() {}
    void setPosition(const CCPoint& pos) { m_obPosition = pos; }
    const CCPoint& getPosition() const { return m_obPosition; }
    float getPositionX() const { return m_obPosition.x; }
    float getPositionY() const { return m_obPosition.y; }
    void setVisible(bool visible) { m_bVisible = visible; }
    bool isVisible() const { return m_bVisible; }
    void setScale(float scale) { m_fScaleX = m_fScaleY = scale; }
    void setRotation(float rot) { m_fRotationX = m_fRotationY = rot; }
    void setOpacity(uint8_t opacity) {}

    float m_fRotationX;
    float m_fRotationY;
    float m_fScaleX;
    float m_fScaleY;
    float m_fVertexZ;
    CCPoint m_obPosition;
    float m_fSkewX;
    float m_fSkewY;
    CCPoint m_obAnchorPointInPoints;
    CCPoint m_obAnchorPoint;
    CCSize m_obContentSize;
    void* m_pCamera;
    void* m_pGrid;
    int m_nZOrder;
    CCArray* m_pChildren;
    CCNode* m_pParent;
    void* m_pUserData;
    int m_nTag;
    int m_nOrderOfArrival;
    void* m_pGLServerState;
    void* m_pShaderProgram;
    int m_eGLServerState;
    bool m_bRunning;
    bool m_bTransformDirty;
    bool m_bInverseDirty;
    bool m_bAdditionalTransformDirty;
    bool m_bVisible;
};

class CCLayer : public CCNode {
public:
    virtual ~CCLayer() {}
};

class CCLayerColor : public CCLayer {
public:
    virtual ~CCLayerColor() {}
};

// Enums from Geometry Dash
enum class GameObjectType {
    Solid = 0,
    Hazard = 2,
    InverseGravityPortal = 3,
    NormalGravityPortal = 4,
    ShipPortal = 5,
    CubePortal = 6,
    Decoration = 7,
    YellowJumpPad = 8,
    PinkJumpPad = 9,
    GravityPad = 10,
    YellowJumpRing = 11,
    PinkJumpRing = 12,
    GravityRing = 13,
    InverseMirrorPortal = 14,
    NormalMirrorPortal = 15,
    BallPortal = 16,
    RegularSizePortal = 17,
    MiniSizePortal = 18,
    UfoPortal = 19,
    Modifier = 20,
    Breakable = 21,
    SecretCoin = 22,
    DualPortal = 23,
    SoloPortal = 24,
    Slope = 25,
    WavePortal = 26,
    RobotPortal = 27,
    TeleportPortal = 28,
    GreenRing = 29,
    Collectible = 30,
    UserCoin = 31,
    DropRing = 32, // Black orb
    SpiderPortal = 33,
    RedJumpPad = 34,
    RedJumpRing = 35,
    CustomRing = 36,
    DashRing = 37,
    GravityDashRing = 38,
    CollisionObject = 39,
    Special = 40
};

enum class PlayerButton {
    Jump = 1,
    Left = 2,
    Right = 3
};

// Game Object
class GameObject : public CCNode {
public:
    virtual ~GameObject() {}

    int getObjectType() const {
        return *reinterpret_cast<const int*>(uintptr_t(this) + 0x300);
    }
    void setObjectType(int t) {
        *reinterpret_cast<int*>(uintptr_t(this) + 0x300) = t;
    }

    int getObjectID() const {
        return *reinterpret_cast<const int*>(uintptr_t(this) + 0x360);
    }
    void setObjectID(int id) {
        *reinterpret_cast<int*>(uintptr_t(this) + 0x360) = id;
    }

    bool isHidden() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x347);
    }
    void setHidden(bool h) {
        *reinterpret_cast<bool*>(uintptr_t(this) + 0x347) = h;
    }

    bool hasNoGlow() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x353);
    }
    void setNoGlow(bool g) {
        *reinterpret_cast<bool*>(uintptr_t(this) + 0x353) = g;
    }

    bool hasNoEffects() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x364);
    }
    void setNoEffects(bool e) {
        *reinterpret_cast<bool*>(uintptr_t(this) + 0x364) = e;
    }

    // GD 2.113 methods
    void addGlow();
    void playShineEffect();
};

class EffectGameObject : public GameObject {
public:
    virtual ~EffectGameObject() {}
};

class RingObject : public EffectGameObject {
public:
    virtual ~RingObject() {}
    void spawnCircle();
};

// Player Object
class PlayerObject : public GameObject {
public:
    virtual ~PlayerObject() {}

    // GD 2.113 verified fields
    bool isHolding() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x611);
    }
    void setHolding(bool h) {
        *reinterpret_cast<bool*>(uintptr_t(this) + 0x611) = h;
    }

    bool hasJustHeld() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x612);
    }
    void setHasJustHeld(bool h) {
        *reinterpret_cast<bool*>(uintptr_t(this) + 0x612) = h;
    }

    double getYVelocity() const {
        return *reinterpret_cast<const double*>(uintptr_t(this) + 0x628);
    }
    void setYVelocity(double v) {
        *reinterpret_cast<double*>(uintptr_t(this) + 0x628) = v;
    }

    bool isShip() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x638);
    }
    bool isBird() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x639);
    }
    bool isBall() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63a);
    }
    bool isDart() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63b);
    }
    bool isRobot() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63c);
    }
    bool isSpider() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63d);
    }
    bool isUpsideDown() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63e);
    }
    bool isOnGround() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x63f);
    }
    bool isDead() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x662);
    }

    CCArray* getTouchingRings() const {
        return *reinterpret_cast<CCArray* const*>(uintptr_t(this) + 0x66c);
    }

    float getPositionX() const {
        return *reinterpret_cast<const float*>(uintptr_t(this) + 0x67c);
    }
    float getPositionY() const {
        return *reinterpret_cast<const float*>(uintptr_t(this) + 0x680);
    }
    void setPositionY(float y) {
        *reinterpret_cast<float*>(uintptr_t(this) + 0x680) = y;
    }

    // 2.113 methods
    void pushButton(int button);
    void releaseButton(int button);
    void playDeathEffect();
    void stopDashing();
    void spawnPortalCircle(ccColor3B color, float radius);
    void propellPlayer(float yVelocity);
    void ringJump(GameObject* obj);
};

// GJBaseGameLayer
class GJBaseGameLayer : public CCLayer {
public:
    virtual ~GJBaseGameLayer() {}

    PlayerObject* getPlayer1() const {
        return *reinterpret_cast<PlayerObject* const*>(uintptr_t(this) + 0x224);
    }
    PlayerObject* getPlayer2() const {
        return *reinterpret_cast<PlayerObject* const*>(uintptr_t(this) + 0x228);
    }
    bool isDualMode() const {
        return *reinterpret_cast<const bool*>(uintptr_t(this) + 0x2cc);
    }

    // 2.113 methods
    void pushButton(int button, bool isPlayer2);
    void releaseButton(int button, bool isPlayer2);
};

// PlayLayer
class PlayLayer : public GJBaseGameLayer {
public:
    virtual ~PlayLayer() {}

    static PlayLayer* get();

    void addObject(GameObject* object);
    void destroyPlayer(PlayerObject* player, GameObject* object);
    void resetLevel();
    void startGame();
    void showEndLayer();
    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle);
    void playGravityEffect(bool flip);
    void onEnterTransitionDidFinish();
    void toggleGlitter(bool enable);
    void update(float dt);
};

// Level Settings
class LevelSettingsObject : public CCObject {
public:
    uint8_t pad[0x100];
    bool m_twoPlayerMode;
};

// CCBlockLayer
class CCBlockLayer : public CCLayerColor {
public:
    virtual ~CCBlockLayer() {}
    virtual void customSetup() {}
    virtual void showLayer(bool instant) {}
};

// Pause Layer
class PauseLayer : public CCBlockLayer {
public:
    virtual ~PauseLayer() {}
    virtual void customSetup();
};

// EndLevelLayer
class EndLevelLayer : public CCLayer {
public:
    virtual ~EndLevelLayer() {}
    void customSetup();
};

// CCCircleWave
class CCCircleWave : public CCNode {
public:
    virtual ~CCCircleWave() {}
    virtual void draw();
};

} // namespace gd
