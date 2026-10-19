#ifndef _Z_BG_INGATE_H_
#define _Z_BG_INGATE_H_

#include "global.hpp"

struct BgIngate;

typedef void (*BgIngateActionFunc)(struct BgIngate* self, PlayState* play);

typedef struct BgIngate {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgIngateActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} BgIngate; // size = 0x1C4

extern ActorInit Bg_Ingate_InitVars;

#endif
