#ifndef _Z_BG_DDAN_KD_H_
#define _Z_BG_DDAN_KD_H_

#include "global.hpp"

struct BgDdanKd;

typedef void (*BgDdanKdActionFunc)(struct BgDdanKd*, PlayState*);

typedef struct BgDdanKd {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ Actor* prevExplosive;
    /* 0x01C0 */ s16 timer;
    /* 0x01C4 */ Vec3f prevExplosivePos;
    /* 0x01D0 */ ColliderCylinder collider;
    /* 0x0228 */ BgDdanKdActionFunc actionFunc;
    /* 0x022C */ SkeletonAnimationModel* skelAnimModel;
} BgDdanKd; // size = 0x0230

extern ActorInit Bg_Ddan_Kd_InitVars;

#endif
