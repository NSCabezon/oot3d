#ifndef _Z_BG_ZG_H_
#define _Z_BG_ZG_H_

#include "global.hpp"

struct BgZg;

typedef void (*BgZgActionFunc)(struct BgZg*, PlayState*);
typedef void (*BgZgDrawFunc)(struct BgZg*, PlayState*);

typedef struct BgZg {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s32 action;
    /* 0x01C0 */ s32 drawConfig;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgZg; // size = 0x1C8

extern ActorInit Bg_Zg_InitVars;

#endif
