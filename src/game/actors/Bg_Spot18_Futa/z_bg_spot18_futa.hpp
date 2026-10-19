#ifndef _Z_BG_SPOT18_FUTA_H_
#define _Z_BG_SPOT18_FUTA_H_

#include "global.hpp"

struct BgSpot18Futa;

typedef struct BgSpot18Futa {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot18Futa; // size = 0x1C0

extern ActorInit Bg_Spot18_Futa_InitVars;

#endif
