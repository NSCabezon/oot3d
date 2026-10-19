#ifndef _Z_BG_HAKA_MEGANE_H_
#define _Z_BG_HAKA_MEGANE_H_

#include "global.hpp"

struct BgHakaMegane;

typedef void (*BgHakaMeganeActionFunc)(struct BgHakaMegane*, PlayState*);

typedef struct BgHakaMegane {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaMeganeActionFunc actionFunc;
    /* 0x01C0 */ char unk_1C0[0x1];
    /* 0x01C1 */ s8 requiredObjectSlot;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel2; // only drawn for params 0 (BotW blood splatter)
} BgHakaMegane; // size = 0x1CC

extern ActorInit Bg_Haka_Megane_InitVars;

#endif
