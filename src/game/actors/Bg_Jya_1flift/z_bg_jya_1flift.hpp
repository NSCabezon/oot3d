#ifndef _Z_BG_JYA_1FLIFT_H_
#define _Z_BG_JYA_1FLIFT_H_

#include "global.hpp"

struct BgJya1flift;

typedef void (*BgJya1fliftActionFunc)(struct BgJya1flift* self, PlayState* play);

typedef struct BgJya1flift {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ColliderCylinder collider;
    /* 0x0214 */ BgJya1fliftActionFunc actionFunc;
    /* 0x0218 */ s16 moveDelay;
    /* 0x021A */ u8 isMovingDown;
    /* 0x021B */ u8 hasInitialized;
    /* 0x021C */ u8 isLinkRiding;
    /* 0x0220 */ SkeletonAnimationModel* skelAnimModel;
} BgJya1flift; // size = 0x224

extern ActorInit Bg_Jya_1flift_InitVars;

#endif
