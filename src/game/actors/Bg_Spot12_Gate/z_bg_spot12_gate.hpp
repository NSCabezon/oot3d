#ifndef _Z_BG_SPOT12_GATE_H_
#define _Z_BG_SPOT12_GATE_H_

#include "global.hpp"

struct BgSpot12Gate;

typedef void (*BgSpot12GateActionFunc)(struct BgSpot12Gate* self, PlayState* play);

typedef struct BgSpot12Gate {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot12GateActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot12Gate; // size = 0x1C8

extern ActorInit Bg_Spot12_Gate_InitVars;

#endif
