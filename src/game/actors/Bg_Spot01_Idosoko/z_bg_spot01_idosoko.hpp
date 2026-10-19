#ifndef _Z_BG_SPOT01_IDOSOKO_H_
#define _Z_BG_SPOT01_IDOSOKO_H_

#include "global.hpp"

struct BgSpot01Idosoko;

typedef void (*BgSpot01IdosokoActionFunc)(struct BgSpot01Idosoko*, PlayState*);

typedef struct BgSpot01Idosoko {
    /* 0x000 */ DynaPolyActor dyna; // bgId at 0x1A4
    /* 0x1BC */ BgSpot01IdosokoActionFunc actionFunc;
    /* 0x1C0 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot01Idosoko; // size = 0x1C4

extern ActorInit Bg_Spot01_Idosoko_InitVars;

#endif
