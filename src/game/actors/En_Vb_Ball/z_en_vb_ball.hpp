#ifndef _Z_EN_VB_BALL_H_
#define _Z_EN_VB_BALL_H_

#include "global.hpp"

struct EnVbBall;

typedef struct EnVbBall {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x4];
    /* 0x01A8 */ s16 unkTimer1; // These count up and down, but nothing uses them.
    /* 0x01AA */ s16 unkTimer2;
    /* 0x01AC */ char unk_1AC[0x4];
    /* 0x01B0 */ f32 xRotVel;
    /* 0x01B4 */ f32 yRotVel;
    /* 0x01B8 */ f32 shadowSize;
    /* 0x01BC */ f32 shadowOpacity;
    /* 0x01C0 */ ColliderCylinder collider;
    /* 0x0218 */ f32 unk_218;
    /* 0x021C */ SkeletonAnimationModel* model0;
    /* 0x0220 */ SkeletonAnimationModel* model1;
    /* 0x0224 */ SkeletonAnimationModel* model2;
} EnVbBall; // size = 0x228

extern ActorInit En_Vb_Ball_InitVars;

#endif
