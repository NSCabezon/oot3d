#ifndef _Z_BG_SPOT17_BAKUDANKABE_H_
#define _Z_BG_SPOT17_BAKUDANKABE_H_

#include "global.hpp"

struct BgSpot17Bakudankabe;

typedef struct BgSpot17Bakudankabe {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot17Bakudankabe; // size = 0x1C0

extern ActorInit Bg_Spot17_Bakudankabe_InitVars;

#endif
