#ifndef _Z_BG_SPOT12_SAKU_H_
#define _Z_BG_SPOT12_SAKU_H_

#include "global.hpp"

struct BgSpot12Saku;

typedef void (*BgSpot12SakuActionFunc)(struct BgSpot12Saku* self, PlayState* play);

typedef struct BgSpot12Saku {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot12SakuActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot12Saku; // size = 0x1C8

extern ActorInit Bg_Spot12_Saku_InitVars;

#endif
