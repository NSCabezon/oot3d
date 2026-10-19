#ifndef _Z_BG_HIDAN_ROCK_H_
#define _Z_BG_HIDAN_ROCK_H_

#include "global.hpp"

struct BgHidanRock;

typedef void (*BgHidanRockActionFunc)(struct BgHidanRock*, PlayState*);

typedef struct BgHidanRock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanRockActionFunc actionFunc;
    /* 0x01C0 */ u8 type;
    /* 0x01C1 */ u8 unk_1C1;
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ f32 unk_1C4;
    /* 0x01C8 */ Vec3f unk_1C8;
    /* 0x01D4 */ ColliderCylinder collider;
    /* 0x022C */ SkeletonAnimationModel* skelAnimModel[3];
} BgHidanRock; // size = 0x238

extern ActorInit Bg_Hidan_Rock_InitVars;

#endif
