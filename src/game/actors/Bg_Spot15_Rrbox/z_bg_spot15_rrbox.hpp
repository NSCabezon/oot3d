#ifndef _Z_BG_SPOT15_RRBOX_H_
#define _Z_BG_SPOT15_RRBOX_H_

#include "global.hpp"

struct BgSpot15Rrbox;

typedef void (*BgSpot15RrboxActionFunc)(struct BgSpot15Rrbox* self, PlayState* play);

typedef struct BgSpot15Rrbox {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot15RrboxActionFunc actionFunc;
    /* 0x01C0 */ s16 unk_168;
    /* 0x01C4 */ f32 unk_16C;
    /* 0x01C8 */ f32 unk_170;
    /* 0x01CC */ f32 unk_174;
    /* 0x01D0 */ f32 unk_178;
    /* 0x01D4 */ f32 unk_17C;
    /* 0x01D8 */ s32 bgId; // Id of BgActor beneath the box
    /* 0x01DC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot15Rrbox; // size = 0x01E0

extern ActorInit Bg_Spot15_Rrbox_InitVars;

#endif
