#ifndef _Z_BG_JYA_BIGMIRROR_H_
#define _Z_BG_JYA_BIGMIRROR_H_

#include "global.hpp"

struct BgJyaBigmirror;

typedef struct BgJyaBigmirror {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x11];
    /* 0x01B5 */ u8 spawned;
    /* 0x01B6 */ char unk_1B6[0x16];
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel3;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel4;
    /* 0x01D4 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x01D8 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x01DC */ void* object;
} BgJyaBigmirror; // size = 0x1E0

extern ActorInit Bg_Jya_Bigmirror_InitVars;

#endif
