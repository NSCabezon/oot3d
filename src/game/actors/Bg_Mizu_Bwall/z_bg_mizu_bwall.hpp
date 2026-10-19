#ifndef _Z_BG_MIZU_BWALL_H_
#define _Z_BG_MIZU_BWALL_H_

#include "global.hpp"

struct BgMizuBwall;

typedef void (*BgMizuBwallActionFunc)(struct BgMizuBwall*, PlayState*);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Bombwall.
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

typedef struct BgMizuBwall {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ColliderTris collider;
    /* 0x01DC */ ColliderTrisElement colliderElements[3];
    /* 0x02F0 */ BgMizuBwallActionFunc actionFunc;
    /* 0x02F4 */ f32 yRot;
    /* 0x02F8 */ s32 scrollAlpha1;
    /* 0x02FC */ s32 scrollAlpha2;
    /* 0x0300 */ s32 scrollAlpha3;
    /* 0x0304 */ s32 scrollAlpha4;
    /* 0x0308 */ s32 breakTimer;
    /* 0x030C */ char unk_30C[4];
    /* 0x0310 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0314 */ s32 unk_314;
} BgMizuBwall; // size = 0x318

extern ActorInit Bg_Mizu_Bwall_InitVars;

#endif
