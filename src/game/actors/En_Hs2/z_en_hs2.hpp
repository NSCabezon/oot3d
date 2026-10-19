#ifndef _Z_EN_HS2_H_
#define _Z_EN_HS2_H_

#include "global.hpp"

struct EnHs2;

typedef void (*EnHs2ActionFunc)(struct EnHs2*, PlayState*);

typedef struct EnHs2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ Vec3s jointTable[16];
    /* 0x02E0 */ char unk_2E0[0x1DC];
    /* 0x04BC */ Vec3s morphTable[16];
    /* 0x051C */ char unk_51C[0x1DC];
    /* 0x06F8 */ Vec3s headRot;
    /* 0x06FE */ Vec3s torsoRot;
    /* 0x0704 */ u16 trackingFlags;
    /* 0x0708 */ EnHs2ActionFunc actionFunc;
    /* 0x070C */ char unk_70C[0x4];
    /* 0x0710 */ SkeletonAnimationModel* skelAnimModel;
} EnHs2; // size = 0x714

extern ActorInit En_Hs2_InitVars;

#endif
