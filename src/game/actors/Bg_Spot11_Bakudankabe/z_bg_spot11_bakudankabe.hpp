#ifndef _Z_BG_SPOT11_BAKUDANKABE_H_
#define _Z_BG_SPOT11_BAKUDANKABE_H_

#include "global.hpp"

struct BgSpot11Bakudankabe;

typedef struct BgSpot11Bakudankabe {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ColliderCylinder collider;
    /* 0x0214 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot11Bakudankabe; // size = 0x218

extern ActorInit Bg_Spot11_Bakudankabe_InitVars;

#endif
