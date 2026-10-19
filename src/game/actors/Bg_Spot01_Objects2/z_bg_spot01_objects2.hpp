#ifndef _Z_BG_SPOT01_OBJECTS2_H_
#define _Z_BG_SPOT01_OBJECTS2_H_

#include "global.hpp"

struct BgSpot01Objects2;

typedef void (*BgSpot01Objects2ActionFunc)(struct BgSpot01Objects2*, PlayState*);

typedef struct BgSpot01Objects2 {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot01Objects2ActionFunc actionFunc;
    /* 0x01C0 */ char unk_1C0[0x10];
    /* 0x01D0 */ s32 objectId;
    /* 0x01D4 */ s8 requiredObjectSlot;
    /* 0x01D5 */ char unk_1D5[0x3];
    /* 0x01D8 */ SkeletonAnimationModel* skelAnimModel[5];
} BgSpot01Objects2; // size = 0x01EC

extern ActorInit Bg_Spot01_Objects2_InitVars;

#endif
