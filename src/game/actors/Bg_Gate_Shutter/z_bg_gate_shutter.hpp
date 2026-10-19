#ifndef _Z_BG_GATE_SHUTTER_H_
#define _Z_BG_GATE_SHUTTER_H_

#include "global.hpp"

struct BgGateShutter;

typedef void (*BgGateShutterActionFunc)(struct BgGateShutter*, PlayState*);

typedef struct BgGateShutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgGateShutterActionFunc actionFunc;
    /* 0x01C0 */ s16 openingState; // 1 if gate is opening
    /* 0x01C4 */ Vec3f somePos;
    /* 0x01D0 */ s16 timer;
    /* 0x01D4 */ SkeletonAnimationModel* skelAnimModel;
} BgGateShutter; // size = 0x01D8

extern ActorInit Bg_Gate_Shutter_InitVars;

#endif
