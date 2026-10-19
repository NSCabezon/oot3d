#ifndef _Z_EN_ISHI_H_
#define _Z_EN_ISHI_H_

#include "global.hpp"

struct EnIshi;

typedef void (*EnIshiActionFunc)(struct EnIshi* self, PlayState* play);

typedef struct EnIshi {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnIshiActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ SkeletonAnimationModel* model;
} EnIshi; // size = 0x204

extern ActorInit En_Ishi_InitVars;

#endif
