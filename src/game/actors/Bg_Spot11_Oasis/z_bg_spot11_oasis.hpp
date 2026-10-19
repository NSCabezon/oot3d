#ifndef _Z_BG_SPOT11_OASIS_H_
#define _Z_BG_SPOT11_OASIS_H_

#include "global.hpp"

struct BgSpot11Oasis;

typedef void (*BgSpot11OasisActionFunc)(struct BgSpot11Oasis*, PlayState*);

typedef struct BgSpot11Oasis {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgSpot11OasisActionFunc actionFunc;
    /* 0x01A8 */ u8 unk_1A8;
    /* 0x01A9 */ u8 bubbleSpawnIdx;
    /* 0x01AA */ char unk_1AA[0x2];
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot11Oasis; // size = 0x1B0

extern ActorInit Bg_Spot11_Oasis_InitVars;

#endif
