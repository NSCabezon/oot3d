#ifndef _Z_BG_ICE_SHUTTER_H_
#define _Z_BG_ICE_SHUTTER_H_

#include "global.hpp"

struct BgIceShutter;

typedef void (*BgIceShutterActionFunc)(struct BgIceShutter*, PlayState*);

typedef struct BgIceShutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgIceShutterActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} BgIceShutter; // size = 0x1C4

extern ActorInit Bg_Ice_Shutter_InitVars;

#endif
