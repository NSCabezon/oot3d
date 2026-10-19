#ifndef _Z_BG_MORI_IDOMIZU_H_
#define _Z_BG_MORI_IDOMIZU_H_

#include "global.hpp"

struct BgMoriIdomizu;

typedef void (*BgMoriIdomizuActionFunc)(struct BgMoriIdomizu*, PlayState*);

typedef struct BgMoriIdomizu {
    /* 0x000 */ Actor actor;
    /* 0x1A4 */ BgMoriIdomizuActionFunc actionFunc;
    /* 0x1A8 */ f32 targetWaterLevel;
    /* 0x1AC */ s32 prevSwitchFlagSet;
    /* 0x1B0 */ s16 isLoaded;
    /* 0x1B2 */ s16 drainTimer;
    /* 0x1B4 */ s8 moriTexObjectSlot;
    /* 0x1B8 */ SkeletonAnimationModel* skelAnimModel;
} BgMoriIdomizu; // size = 0x1BC

extern ActorInit Bg_Mori_Idomizu_InitVars;

#endif
