#ifndef _Z_BG_SPOT15_SAKU_H_
#define _Z_BG_SPOT15_SAKU_H_

#include "global.hpp"

struct BgSpot15Saku;

typedef void (*BgSpot15SakuActionFunc)(struct BgSpot15Saku*, PlayState*);

typedef struct BgSpot15Saku {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot15SakuActionFunc actionFunc;
    /* 0x01C0 */ u64 unk_168;
    /* 0x01C8 */ Vec3f unk_170;
    /* 0x01D4 */ s16 timer;
    /* 0x01D8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01DC */ char unk_1DC[0x4];
} BgSpot15Saku; // size = 0x01E0

extern ActorInit Bg_Spot15_Saku_InitVars;

#endif
