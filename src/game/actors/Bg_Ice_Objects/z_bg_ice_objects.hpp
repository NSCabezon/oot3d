#ifndef _Z_BG_ICE_OBJECTS_H_
#define _Z_BG_ICE_OBJECTS_H_

#include "global.hpp"

struct BgIceObjects;

typedef void (*BgIceObjectsActionFunc)(struct BgIceObjects*, PlayState*);

typedef struct BgIceObjects {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgIceObjectsActionFunc actionFunc;
    /* 0x01C0 */ Vec3f targetPos;
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel;
} BgIceObjects; // size = 0x1D0

extern ActorInit Bg_Ice_Objects_InitVars;

#endif
