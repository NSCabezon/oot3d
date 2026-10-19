#ifndef _Z_BG_HAKA_SHIP_H_
#define _Z_BG_HAKA_SHIP_H_

#include "global.hpp"

struct BgHakaShip;

typedef void (*BgHakaShipActionFunc)(struct BgHakaShip*, PlayState*);

typedef struct BgHakaShip {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaShipActionFunc actionFunc;
    /* 0x01C0 */ u8 counter;
    /* 0x01C1 */ u8 switchFlag;
    /* 0x01C2 */ s16 yOffset;
    /* 0x01C4 */ Vec3f bellSfxPos;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel0;
    /* 0x01D4 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x01D8 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x01DC */ SkeletonAnimationModel* skelAnimModel3;
} BgHakaShip; // size = 0x01E0

extern ActorInit Bg_Haka_Ship_InitVars;

#endif
