#ifndef _Z_BG_JYA_BLOCK_H_
#define _Z_BG_JYA_BLOCK_H_

#include "global.hpp"

struct BgJyaBlock;

typedef struct BgJyaBlock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
} BgJyaBlock; // size = 0x1C0

extern ActorInit Bg_Jya_Block_InitVars;

#endif
