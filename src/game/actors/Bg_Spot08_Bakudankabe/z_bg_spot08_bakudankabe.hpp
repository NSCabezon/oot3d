#ifndef _Z_BG_SPOT08_BAKUDANKABE_H_
#define _Z_BG_SPOT08_BAKUDANKABE_H_

#include "global.hpp"

struct BgSpot08Bakudankabe;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u8 unk_00[0x50];
} BgSpot08BakudankabeColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgSpot08BakudankabeColliderElement* elements;
} BgSpot08BakudankabeColliderJntSph; // size = 0x20

typedef struct BgSpot08Bakudankabe {
    /* 0x0000 */ DynaPolyActor dyna; // dyna.bgId at 0x01A4
    /* 0x01BC */ BgSpot08BakudankabeColliderJntSph collider;
    /* 0x01DC */ BgSpot08BakudankabeColliderElement colliderElements[3];
    /* 0x02CC */ SkeletonAnimationModel* skelAnimModel;
} BgSpot08Bakudankabe; // size = 0x2D0

extern ActorInit Bg_Spot08_Bakudankabe_InitVars;

#endif
