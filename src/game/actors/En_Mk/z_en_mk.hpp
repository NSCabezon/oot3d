#ifndef _Z_EN_MK_H_
#define _Z_EN_MK_H_

#include "global.hpp"

struct EnMk;

typedef void (*EnMkActionFunc)(struct EnMk*, PlayState*);

typedef struct EnMk {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ Vec3s jointTable[14];
    /* 0x02D4 */ char unk_2D4[0x284];
    /* 0x0558 */ Vec3s morphTable[14];
    /* 0x05AC */ char unk_5AC[0x284];
    /* 0x0830 */ Vec3s headRotation;
    /* 0x0836 */ u16 flags;
    /* 0x0838 */ s16 swimFlag;
    /* 0x083A */ u16 timer;
    /* 0x083C */ EnMkActionFunc actionFunc;
    /* 0x0840 */ SkeletonAnimationModel* skelAnimModel;
} EnMk; // size = 0x844

extern ActorInit En_Mk_InitVars;

#endif
