#ifndef _Z_OCEFF_SPOT_H_
#define _Z_OCEFF_SPOT_H_

#include "global.hpp"

struct OceffSpot;

typedef void (*OceffSpotActionFunc)(struct OceffSpot* self, PlayState* play);

typedef struct OceffSpot {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void* lightNode1;
    /* 0x01A8 */ char unk_1A8[0x18];
    /* 0x01C0 */ void* lightNode2;
    /* 0x01C4 */ char unk_1C4[0x18];
    /* 0x01DC */ f32 unk_174; // sun's song cylinder progress, 0..1
    /* 0x01E0 */ u16 timer;
    /* 0x01E4 */ OceffSpotActionFunc actionFunc;
    /* 0x01E8 */ SkeletonAnimationModel* model;
    /* 0x01EC */ void* unk_1EC;
} OceffSpot; // size = 0x1F0

extern ActorInit Oceff_Spot_InitVars;

#endif
