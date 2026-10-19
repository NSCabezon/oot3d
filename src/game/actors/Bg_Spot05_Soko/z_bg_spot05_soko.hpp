#ifndef _Z_BG_SPOT05_SOKO_H_
#define _Z_BG_SPOT05_SOKO_H_

#include "global.hpp"

struct BgSpot05Soko;

typedef void (*BgSpot05SokoActionFunc)(struct BgSpot05Soko* self, PlayState* play);

typedef struct BgSpot05Soko {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot05SokoActionFunc actionFunc;
    /* 0x01C0 */ s32 switchFlag;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel2;
} BgSpot05Soko; // size = 0x1CC

extern ActorInit Bg_Spot05_Soko_InitVars;

#endif
