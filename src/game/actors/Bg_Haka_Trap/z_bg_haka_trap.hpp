#ifndef _Z_BG_HAKA_TRAP_H_
#define _Z_BG_HAKA_TRAP_H_

#include "global.hpp"

struct BgHakaTrap;

typedef void (*BgHakaTrapActionFunc)(struct BgHakaTrap* self, PlayState* play);

// Collision types not yet in the shared headers (3DS layout).
typedef struct {
    /* 0x00 */ u8 bytes[0x5C];
} ColliderTrisElement;

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct BgHakaTrap {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaTrapActionFunc actionFunc;
    /* 0x01C0 */ u8 timer;
    /* 0x01C1 */ u8 isSpikedCrusherStationary;
    /* 0x01C2 */ union {
        s16 spikedCrusherStepY;
        u16 isGuillotineFast;
    };
    /* 0x01C4 */ Vec3f chainLiftSfxPos;
    /* 0x01D0 */ ColliderCylinder colliderCylinder;
    /* 0x0228 */ ColliderTris spikesCollider;
    /* 0x0248 */ ColliderTrisElement spikesColliderElements[2];
    /* 0x0300 */ struct SkeletonAnimationModel* skelAnimModel[5];
    /* 0x0314 */ s32 unk_314;
    /* 0x0318 */ char unk_318[0x4];
} BgHakaTrap; // size = 0x31C

extern ActorInit Bg_Haka_Trap_InitVars;

#endif
