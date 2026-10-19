#ifndef _Z_BG_SPOT08_ICEBLOCK_H_
#define _Z_BG_SPOT08_ICEBLOCK_H_

#include "global.hpp"

struct BgSpot08Iceblock;

typedef void (*BgSpot08IceblockActionFunc)(struct BgSpot08Iceblock* self, PlayState* play);

typedef struct BgSpot08Iceblock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot08IceblockActionFunc actionFunc;
    /* 0x01C0 */ char unk_1C0[4];
    /* 0x01C4 */ f32 unk_1C4;
    /* 0x01C8 */ char unk_1C8[0x10];
    /* 0x01D8 */ f32 unk_1D8;
    /* 0x01DC */ char unk_1DC[8];
    /* 0x01E4 */ s16 bobPhaseSlow;
    /* 0x01E6 */ s16 bobPhaseFast;
    /* 0x01E8 */ s16 bobIncrSlow;
    /* 0x01EA */ s16 bobIncrFast;
    /* 0x01EC */ char unk_1EC[0x8];
    /* 0x01F4 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x01F8 */ SkeletonAnimationModel* skelAnimModel2;
} BgSpot08Iceblock; // size = 0x1FC

extern ActorInit Bg_Spot08_Iceblock_InitVars;

#endif
