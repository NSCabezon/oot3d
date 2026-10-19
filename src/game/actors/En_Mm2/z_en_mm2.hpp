#ifndef _Z_EN_MM2_H_
#define _Z_EN_MM2_H_

#include "global.hpp"

struct EnMm2;

typedef void (*EnMm2ActionFunc)(struct EnMm2*, PlayState*);

typedef struct EnMm2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x30C];
    /* 0x0534 */ char morphTable[0x2CC];
    /* 0x0800 */ char unk_800[0x40];
    /* 0x0840 */ EnMm2ActionFunc actionFunc;
    /* 0x0844 */ ColliderCylinder collider;
    /* 0x089C */ s32 mouthTexIndex;
    /* 0x08A0 */ char unk_8A0[0x4];
    /* 0x08A4 */ Vec3s unk_8A4;
    /* 0x08AA */ Vec3s unk_8AA;
    /* 0x08B0 */ u16 unk_8B0;
    /* 0x08B2 */ s16 unk_8B2;
    /* 0x08B4 */ s32 previousAnimation;
    /* 0x08B8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x08BC */ FaceAnimations faceAnim;
} EnMm2; // size = 0xA88

extern ActorInit En_Mm2_InitVars;

#endif
