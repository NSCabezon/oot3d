#ifndef _Z_BG_VB_SIMA_H_
#define _Z_BG_VB_SIMA_H_

#include "global.hpp"

struct BgVbSima;

typedef struct BgVbSima {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ char unk_1BC[0x10];
    /* 0x01CC */ u16 shakeTimer;
    /* 0x01CE */ char unk_1CE[0x6];
    /* 0x01D4 */ SkeletonAnimationModel* skelAnimModel;
} BgVbSima; // size = 0x1D8

extern ActorInit Bg_Vb_Sima_InitVars;

#endif
