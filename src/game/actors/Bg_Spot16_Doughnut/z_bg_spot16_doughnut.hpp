#ifndef _Z_BG_SPOT16_DOUGHNUT_H_
#define _Z_BG_SPOT16_DOUGHNUT_H_

#include "global.hpp"

struct BgSpot16Doughnut;

typedef struct BgSpot16Doughnut {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u16 fireFlag; // bit 0: fiery doughnut (cutscene-driven fade)
    /* 0x01A6 */ char unk_1A6[0x2];
    /* 0x01A8 */ u8 envColorAlpha;
    /* 0x01A9 */ char unk_1A9[0x3];
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B0 */ SkeletonAnimationModel* skelAnimModel2;
} BgSpot16Doughnut; // size = 0x1B4

extern ActorInit Bg_Spot16_Doughnut_InitVars;

#endif
