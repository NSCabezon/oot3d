#ifndef _Z_BG_HIDAN_FSLIFT_H_
#define _Z_BG_HIDAN_FSLIFT_H_

#include "global.hpp"

struct BgHidanFslift;

typedef void (*BgHidanFsliftActionFunc)(struct BgHidanFslift*, PlayState*);

typedef struct BgHidanFslift {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanFsliftActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ s16 cameraSetting;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgHidanFslift; // size = 0x1C8

extern ActorInit Bg_Hidan_Fslift_InitVars;

#endif
