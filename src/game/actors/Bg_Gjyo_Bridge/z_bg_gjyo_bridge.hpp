#ifndef _Z_BG_GJYO_BRIDGE_H_
#define _Z_BG_GJYO_BRIDGE_H_

#include "global.hpp"

struct BgGjyoBridge;

typedef void (*BgGjyoBridgeActionFunc)(struct BgGjyoBridge*, PlayState*);

typedef struct BgGjyoBridge {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgGjyoBridgeActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} BgGjyoBridge; // size = 0x01C4

extern ActorInit Bg_Gjyo_Bridge_InitVars;

#endif
