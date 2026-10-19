#ifndef _Z_EN_DY_EXTRA_H_
#define _Z_EN_DY_EXTRA_H_

#include "global.hpp"

struct EnDyExtra;

typedef void (*EnDyExtraActionFunc)(struct EnDyExtra*, PlayState*);

typedef struct EnDyExtra {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDyExtraActionFunc actionFunc;
    /* 0x01A8 */ s16 type;
    /* 0x01AA */ s16 startDisappearing;
    /* 0x01AC */ s16 timer;
    /* 0x01B0 */ f32 alphaFac;
    /* 0x01B4 */ Vec3f scale;
    /* 0x01C0 */ Vec3f homePos;
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel;
} EnDyExtra; // size = 0x1D0

extern ActorInit En_Dy_Extra_InitVars;

#endif
