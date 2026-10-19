#ifndef _Z_BG_HAKA_WATER_H_
#define _Z_BG_HAKA_WATER_H_

#include "global.hpp"

struct BgHakaWater;

typedef void (*BgHakaWaterActionFunc)(struct BgHakaWater*, PlayState*);

typedef struct BgHakaWater {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgHakaWaterActionFunc actionFunc;
    /* 0x01A8 */ u8 isLowered;
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B0 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x01B4 */ char unk_1B4[0xC];
} BgHakaWater; // size = 0x1C0

extern ActorInit Bg_Haka_Water_InitVars;

#endif
