#ifndef _Z_BG_SPOT18_SHUTTER_H_
#define _Z_BG_SPOT18_SHUTTER_H_

#include "global.hpp"

struct BgSpot18Shutter;

typedef void (*BgSpot18ShutterActionFunc)(struct BgSpot18Shutter*, PlayState*);

typedef struct BgSpot18Shutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot18ShutterActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot18Shutter; // size = 0x01C4

extern ActorInit Bg_Spot18_Shutter_InitVars;

#endif
