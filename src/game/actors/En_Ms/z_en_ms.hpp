#ifndef _Z_EN_MS_H_
#define _Z_EN_MS_H_

#include "global.hpp"

struct EnMs;

typedef void (*EnMsActionFunc)(struct EnMs*, PlayState*);

typedef struct EnMs {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[12];
    /* 0x0270 */ char unk_270[0x228];
    /* 0x0498 */ Vec3s morphTable[12];
    /* 0x04E0 */ char unk_4E0[0x228];
    /* 0x0708 */ EnMsActionFunc actionFunc;
    /* 0x070C */ ColliderCylinder collider;
    /* 0x0764 */ s16 activeTimer;
    /* 0x0766 */ char unk_766[0x2];
    /* 0x0768 */ SkeletonAnimationModel* skelAnimModel;
} EnMs; // size = 0x76C

extern ActorInit En_Ms_InitVars;

#endif
