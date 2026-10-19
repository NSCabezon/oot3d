#ifndef _Z_OBJ_HAMISHI_H_
#define _Z_OBJ_HAMISHI_H_

#include "global.hpp"

struct ObjHamishi;

typedef struct ObjHamishi {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ f32 shakePosSize;
    /* 0x0200 */ f32 shakeRotSize;
    /* 0x0204 */ s16 shakeFrames;
    /* 0x0206 */ s16 shakePosPhase;
    /* 0x0208 */ s16 shakeRotPhase;
    /* 0x020A */ s16 hitCount;
    /* 0x020C */ SkeletonAnimationModel* model;
} ObjHamishi; // size = 0x210

extern ActorInit Obj_Hamishi_InitVars;

#endif
