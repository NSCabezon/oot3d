#ifndef _Z_BG_SPOT00_BREAK_H_
#define _Z_BG_SPOT00_BREAK_H_

#include "global.hpp"

struct BgSpot00Break;

typedef struct BgSpot00Break {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;      // params == 1 model (index 1)
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel2;     // default model (index 0)
} BgSpot00Break; // size = 0x1C4

extern ActorInit Bg_Spot00_Break_InitVars;

#endif
