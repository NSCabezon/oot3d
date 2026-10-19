#ifndef _Z_EN_HS_H_
#define _Z_EN_HS_H_

#include "global.hpp"

struct EnHs;

typedef void (*EnHsActionFunc)(struct EnHs*, PlayState*);

typedef struct EnHs {
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
    /* 0x0706 */ s16 cojiroSfxTimer;
    /* 0x0708 */ EnHsActionFunc actionFunc;
    /* 0x070C */ u8 unk_70C;
    /* 0x070D */ char unk_70D[0x3];
    /* 0x0710 */ SkeletonAnimationModel* skelAnimModel;
} EnHs; // size = 0x714

extern ActorInit En_Hs_InitVars;

#endif
