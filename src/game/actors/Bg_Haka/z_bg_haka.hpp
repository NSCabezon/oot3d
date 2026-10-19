#ifndef _Z_BG_HAKA_H_
#define _Z_BG_HAKA_H_

#include "global.hpp"

struct BgHaka;

typedef void (*BgHakaActionFunc)(struct BgHaka*, PlayState*);

typedef struct BgHaka {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x01C8 */ s32 unk_1C8; // set when params has bit 0x8000 (cleared from params in Init)
} BgHaka; // size = 0x1CC

extern ActorInit Bg_Haka_InitVars;

#endif
