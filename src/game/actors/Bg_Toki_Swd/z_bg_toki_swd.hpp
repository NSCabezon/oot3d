#ifndef _Z_BG_TOKI_SWD_H_
#define _Z_BG_TOKI_SWD_H_

#include "global.hpp"

struct BgTokiSwd;

typedef void (*BgTokiSwdActionFunc)(struct BgTokiSwd*, PlayState*);

typedef struct BgTokiSwd {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgTokiSwdActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ SkeletonAnimationModel* skelAnimModel;
} BgTokiSwd; // size = 0x204

extern ActorInit Bg_Toki_Swd_InitVars;

#endif
