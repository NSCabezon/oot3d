#ifndef _Z_EN_KUSA_H_
#define _Z_EN_KUSA_H_

#include "global.hpp"

struct EnKusa;

typedef void (*EnKusaActionFunc)(struct EnKusa*, PlayState*);

typedef struct EnKusa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnKusaActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ u16 timer;
    /* 0x0202 */ s8 requiredObjectSlot;
    /* 0x0203 */ char unk_203[0x1];
    /* 0x0204 */ void* models[3]; // SkeletonAnimationModel* / Model*, see FUN_001f7510
} EnKusa; // size = 0x0210

extern ActorInit En_Kusa_InitVars;

#endif
