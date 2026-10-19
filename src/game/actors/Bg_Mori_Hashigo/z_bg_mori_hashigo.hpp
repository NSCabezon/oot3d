#ifndef _Z_BG_MORI_HASHIGO_H_
#define _Z_BG_MORI_HASHIGO_H_

#include "global.hpp"

struct BgMoriHashigo;

typedef void (*BgMoriHashigoActionFunc)(struct BgMoriHashigo*, PlayState*);

// JntSph collider layout (not in include/ yet)
typedef struct BgMoriHashigoJntSph {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C 0x20 */ void* elements;
} BgMoriHashigoJntSph;

typedef struct BgMoriHashigoJntSphElement {
    /* 0x00 */ char unk_00[0x38];
    /* 0x38 */ f32 centerX;
    /* 0x3C */ f32 centerY;
    /* 0x40 */ f32 centerZ;
    /* 0x44 */ f32 radius;
    /* 0x48 */ char unk_48[0x8];
} BgMoriHashigoJntSphElement; // size = 0x50

typedef struct BgMoriHashigo {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriHashigoJntSph collider;
    /* 0x01DC */ BgMoriHashigoJntSphElement colliderElements[1];
    /* 0x022C */ BgMoriHashigoActionFunc actionFunc;
    /* 0x0230 */ s16 hitTimer;
    /* 0x0232 */ s16 bounceCounter;
    /* 0x0234 */ s8 moriTexObjectSlot;
    /* 0x0235 */ char unk_235[0x3];
    /* 0x0238 */ SkeletonAnimationModel* claspSkelAnimModel;
    /* 0x023C */ SkeletonAnimationModel* ladderSkelAnimModel;
} BgMoriHashigo; // size = 0x240

extern ActorInit Bg_Mori_Hashigo_InitVars;

#endif
