#ifndef _Z_BG_SPOT07_TAKI_H_
#define _Z_BG_SPOT07_TAKI_H_

#include "global.hpp"

struct BgSpot07Taki;

typedef void (*BgSpot07TakiActionFunc)(struct BgSpot07Taki*, PlayState*);

typedef struct BgSpot07Taki {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId;
    /* 0x01A8 */ char unk_1A8[0x14];
    /* 0x01BC */ BgSpot07TakiActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel[6];
} BgSpot07Taki; // size = 0x01D8

extern ActorInit Bg_Spot07_Taki_InitVars;

#endif
