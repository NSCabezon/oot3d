#ifndef _Z_EN_XC_H_
#define _Z_EN_XC_H_

#include "global.hpp"

struct EnXc;

typedef struct EnXc {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x3DC];
    /* 0x0604 */ char unk_604[0x3A8];
    /* 0x09AC */ char unk_9AC[0x34];
    /* 0x09E0 */ char unk_9E0[0x1CC]; // FaceAnimations (see EnXc_GetFaceAnim)
    /* 0x0BAC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0BB0 */ char unk_BB0[0x8];
    /* 0x0BB8 */ s32 action;
    /* 0x0BBC */ s32 drawMode;
    /* 0x0BC0 */ f32 timer;
    /* 0x0BC4 */ char unk_BC4[0x38];
    /* 0x0BFC */ u16 unk_BFC;
    /* 0x0BFE */ char unk_BFE[0x1A];
    /* 0x0C18 */ ColliderCylinder collider;
    /* 0x0C70 */ s32 unk_C70;
    /* 0x0C74 */ char unk_C74[0xCA0 - 0xC74];
} EnXc; // size = 0xCA0

extern ActorInit En_Xc_InitVars;

#endif
