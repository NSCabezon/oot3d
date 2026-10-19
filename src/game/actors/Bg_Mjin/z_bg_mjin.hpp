#ifndef _Z_BG_MJIN_H_
#define _Z_BG_MJIN_H_

#include "global.hpp"

struct BgMjin;

typedef void (*BgMjinActionFunc)(struct BgMjin*, PlayState*);

typedef struct BgMjin {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s8 requiredObjectSlot;
    /* 0x01C0 */ BgMjinActionFunc actionFunc;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgMjin; // size = 0x1C8

extern ActorInit Bg_Mjin_InitVars;

#endif
