#ifndef _Z_BG_SPOT09_OBJ_H_
#define _Z_BG_SPOT09_OBJ_H_

#include "global.hpp"

struct BgSpot09Obj;

typedef struct BgSpot09Obj {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ char unk_1BC[0x04];
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel[6];
} BgSpot09Obj; // size = 0x1D8

extern ActorInit Bg_Spot09_Obj_InitVars;

#endif
