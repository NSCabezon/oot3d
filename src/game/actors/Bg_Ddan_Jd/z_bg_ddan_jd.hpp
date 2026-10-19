#ifndef _Z_BG_DDAN_JD_H_
#define _Z_BG_DDAN_JD_H_

#include "global.hpp"

struct BgDdanJd;

typedef void (*BgDdanJdActionFunc)(struct BgDdanJd*, PlayState*);

typedef struct BgDdanJd {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgDdanJdActionFunc actionFunc;
    /* 0x01C0 */ u8 state;
    /* 0x01C1 */ u8 ySpeed; // also differentiates between normal and shortcut platform behavior
    /* 0x01C2 */ s16 idleTimer;
    /* 0x01C4 */ f32 targetY;
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel;
} BgDdanJd; // size = 0x01CC

extern ActorInit Bg_Ddan_Jd_InitVars;

#endif
