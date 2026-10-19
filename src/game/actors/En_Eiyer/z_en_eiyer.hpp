#ifndef _Z_EN_EIYER_H_
#define _Z_EN_EIYER_H_

#include "global.hpp"

struct EnEiyer;

typedef void (*EnEiyerActionFunc)(struct EnEiyer*, PlayState*);

typedef struct EnEiyer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelanime;
    /* 0x0228 */ Vec3s jointTable[14];
    /* 0x027C */ char unk_27C[0x284];
    /* 0x0500 */ Vec3s morphTable[14];
    /* 0x0554 */ char unk_554[0x1AC];
    /* 0x0700 */ char unk_700[0xD8];
    /* 0x07D8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x07DC */ EnEiyerActionFunc actionFunc;
    /* 0x07E0 */ s16 timer;
    /* 0x07E2 */ s16 targetYaw;
    /* 0x07E4 */ Vec3f basePos;
    /* 0x07F0 */ ColliderCylinder collider;
} EnEiyer; // size = 0x0848

extern ActorInit En_Eiyer_InitVars;

#endif
