#ifndef _Z_BG_HAKA_SGAMI_H_
#define _Z_BG_HAKA_SGAMI_H_

#include "global.hpp"

struct BgHakaSgami;

typedef void (*BgHakaSgamiActionFunc)(struct BgHakaSgami*, PlayState*);

// Collision types not yet in the shared headers (3DS layout).
typedef struct {
    /* 0x00 */ Vec3f vtx[3];
} ColliderTrisElementDimInit; // size = 0x24

typedef struct {
    /* 0x00 */ ColliderInfoInit info;
    /* 0x18 */ ColliderTrisElementDimInit dim;
} ColliderTrisElementInit; // size = 0x3C

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInit; // size = 0x10

typedef struct {
    /* 0x00 */ u8 bytes[0x5C];
} ColliderTrisElement;

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct BgHakaSgami {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgHakaSgamiActionFunc actionFunc;
    /* 0x01A8 */ s8 requiredObjectSlot;
    /* 0x01A9 */ s8 unk_151; // params bits 0..7, nonzero = lens-reactive ("unk_151" in N64 source)
    /* 0x01AA */ s16 timer;
    /* 0x01AC */ s32 blureEffectIndex[2];
    /* 0x01B4 */ ColliderCylinder scytheCenterCollider;
    /* 0x020C */ ColliderTris scytheCollider;
    /* 0x022C */ ColliderTrisElement scytheColliderElements[4];
    /* 0x039C */ SkeletonAnimationModel* skelAnimModel;
} BgHakaSgami; // size = 0x3A0

extern ActorInit Bg_Haka_Sgami_InitVars;

#endif
