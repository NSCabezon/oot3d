#ifndef _Z_EN_ARROW_H_
#define _Z_EN_ARROW_H_
#include "global.hpp"
#include <stddef.h>
struct EnArrow;
typedef void (*EnArrowActionFunc)(EnArrow*, PlayState*);
typedef struct {
    Collider base;
    ColliderInfo info;
    char dimensions[0x40];
} EnArrowColliderQuad;
typedef struct {
    s32 active;
    Vec3f posA;
    char unk_10[0xC];
    Vec3f posB;
    char unk_28[0xC];
} EnArrowWeaponInfo;
typedef struct EnArrow {
    Actor actor;
    SkelAnime skelAnime;
    void* textureModel;
    void* managedModel;
    u8 drawEnabled;
    u8 hitSuppressed;
    char unk_232[2];
    EnArrowColliderQuad collider;
    Vec3f previousPos;
    Vec3f flamePos;
    s32 effectIndex;
    EnArrowWeaponInfo weaponInfo;
    u8 timer;
    u8 hitFlags;
    u8 touchedPoly;
    u8 isCsNut;
    Actor* hitActor;
    Vec3f hitOffset;
    EnArrowActionFunc actionFunc;
} EnArrow;
typedef char EnArrowSizeCheck[(sizeof(EnArrow) == 0x31C) ? 1 : -1];
typedef char EnArrowColliderSizeCheck[(sizeof(EnArrowColliderQuad) == 0x80) ? 1 : -1];
typedef char EnArrowWeaponInfoSizeCheck[(sizeof(EnArrowWeaponInfo) == 0x34) ? 1 : -1];
typedef char EnArrowColliderOffsetCheck[(offsetof(EnArrow, collider) == 0x234) ? 1 : -1];
typedef char EnArrowTimerOffsetCheck[(offsetof(EnArrow, timer) == 0x304) ? 1 : -1];
typedef char EnArrowHitActorOffsetCheck[(offsetof(EnArrow, hitActor) == 0x308) ? 1 : -1];
typedef char EnArrowActionOffsetCheck[(offsetof(EnArrow, actionFunc) == 0x318) ? 1 : -1];
extern ActorInit En_Arrow_InitVars;
#endif
