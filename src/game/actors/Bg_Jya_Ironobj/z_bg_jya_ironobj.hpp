#ifndef _Z_BG_JYA_IRONOBJ_H_
#define _Z_BG_JYA_IRONOBJ_H_

#include "global.hpp"

struct BgJyaIronobj;

typedef void (*BgJyaIronobjActionFunc)(struct BgJyaIronobj*, PlayState*);

typedef struct BgJyaIronobj {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaIronobjActionFunc actionFunc;
    /* 0x01C0 */ ColliderCylinder colliderCylinder;
    /* 0x0218 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaIronobj; // size = 0x21C

extern ActorInit Bg_Jya_Ironobj_InitVars;

#endif
