#ifndef _Z_BG_JYA_AMISHUTTER_H_
#define _Z_BG_JYA_AMISHUTTER_H_

#include "global.hpp"

struct BgJyaAmishutter;

typedef void (*BgJyaAmishutterActionFunc)(struct BgJyaAmishutter*);

typedef struct BgJyaAmishutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaAmishutterActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaAmishutter; // size = 0x1C4

extern ActorInit Bg_Jya_Amishutter_InitVars;

#endif
