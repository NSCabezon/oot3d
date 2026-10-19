#ifndef _Z_BG_SPOT01_IDOHASHIRA_H_
#define _Z_BG_SPOT01_IDOHASHIRA_H_

#include "global.hpp"

struct BgSpot01Idohashira;

typedef void (*BgSpot01IdohashiraActionFunc)(struct BgSpot01Idohashira*, PlayState*);

typedef struct BgSpot01Idohashira {
    /* 0x000 */ DynaPolyActor dyna; // bgId at 0x1A4
    /* 0x1BC */ s32 action;
    /* 0x1C0 */ s32 drawConfig;
    /* 0x1C4 */ s32 cueId;
    /* 0x1C8 */ s32 unk_1C8;
    /* 0x1CC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot01Idohashira; // size = 0x1D0

extern ActorInit Bg_Spot01_Idohashira_InitVars;

#endif
