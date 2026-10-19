#ifndef _Z_BG_SST_FLOOR_H_
#define _Z_BG_SST_FLOOR_H_

#include "global.hpp"

struct BgSstFloor;

typedef enum BgSstFloorParams {
    /* 0 */ BONGOFLOOR_REST,
    /* 1 */ BONGOFLOOR_HIT
} BgSstFloorParams;

typedef struct BgSstFloor {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s16 drumPhase;
    /* 0x01BE */ s16 drumAmp;
    /* 0x01C0 */ s16 drumHeight;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ CollisionHeader* colHeader;
} BgSstFloor; // size = 0x01CC

extern ActorInit Bg_Sst_Floor_InitVars;

#endif
