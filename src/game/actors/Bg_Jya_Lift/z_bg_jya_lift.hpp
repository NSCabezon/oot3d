#ifndef _Z_BG_JYA_LIFT_H_
#define _Z_BG_JYA_LIFT_H_

#include "global.hpp"

struct BgJyaLift;

typedef void (*BgJyaLiftActionFunc)(struct BgJyaLift*, PlayState*);

typedef struct BgJyaLift {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaLiftActionFunc actionFunc;
    /* 0x01C0 */ s16 moveDelay;
    /* 0x01C2 */ u8 isSpawned;
    /* 0x01C3 */ u8 prevInteractFlags;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaLift; // size = 0x1C8

extern ActorInit Bg_Jya_Lift_InitVars;

#endif
