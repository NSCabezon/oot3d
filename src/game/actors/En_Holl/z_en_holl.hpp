#ifndef _Z_EN_HOLL_H_
#define _Z_EN_HOLL_H_

#include "global.hpp"

struct EnHoll;

typedef void (*EnHollActionFunc)(struct EnHoll* self, PlayState* play);

typedef struct EnHoll {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 planeAlpha;
    /* 0x01A6 */ u8 side;
    /* 0x01A7 */ u8 resetBgCoverAlpha;
    /* 0x01A8 */ EnHollActionFunc actionFunc;
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B0 */ s8 unk_1B0;
} EnHoll; // size = 0x1B4

extern ActorInit En_Holl_InitVars;

#endif
