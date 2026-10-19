#ifndef _Z_BG_BOMBWALL_H_
#define _Z_BG_BOMBWALL_H_

#include "global.hpp"

struct BgBombwall;

typedef void (*BgBombwallActionFunc)(struct BgBombwall*, PlayState*);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Haka_Sgami / Bg_Ydan_Maruta.
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

typedef struct BgBombwall {
    /* 0x0000 */ DynaPolyActor dyna; // dyna.actor = Actor, dyna.bgId at 0x1A4
    /* 0x01BC */ ColliderTris collider;
    /* 0x01DC */ ColliderTrisElement colliderElements[3];
    /* 0x02F0 */ BgBombwallActionFunc actionFunc;
    /* 0x02F4 */ s16 timer; // N64 unk_2A0
    /* 0x02F6 */ u8 colFlags; // bit 1: DynaPoly bg registered (2), bit 0: tris collider set up (1) (N64 unk_2A2)
    /* 0x02F7 */ u8 unk_2F7;
    /* 0x02F8 */ u32 unk_2F8;
    /* 0x02FC */ SkeletonAnimationModel* skelAnimModel; // active model (N64 dList)
    /* 0x0300 */ SkeletonAnimationModel* skelAnimModelIntact;
    /* 0x0304 */ SkeletonAnimationModel* skelAnimModelBroken;
} BgBombwall; // size = 0x308

extern ActorInit Bg_Bombwall_InitVars;

#endif
