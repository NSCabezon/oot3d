#ifndef _Z_EN_FD_FIRE_H_
#define _Z_EN_FD_FIRE_H_

#include "global.hpp"

struct EnFdFire;

typedef void (*EnFdFireActionFunc)(struct EnFdFire* self, PlayState* play);

typedef struct EnFdFire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnFdFireActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ f32 spawnRadius;
    /* 0x0204 */ f32 scale;
    /* 0x0208 */ char unk_208[2];
    /* 0x020A */ s16 deathTimer;
    /* 0x020C */ s16 tile2Y;
    /* 0x020E */ char unk_20E[2];
    /* 0x0210 */ char unk_210[0xC]; // model object (FUN_0034f94c ctor, FUN_0035046c dtor)
} EnFdFire; // size = 0x21C

extern ActorInit En_Fd_Fire_InitVars;

#endif
