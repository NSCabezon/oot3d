#ifndef _Z_EN_ANUBICE_FIRE_H_
#define _Z_EN_ANUBICE_FIRE_H_

#include "global.hpp"

struct EnAnubiceFire;

typedef struct EnAnubiceFire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void* actionFunc;
    /* 0x01A8 */ f32 unk_1A8;
    /* 0x01AC */ f32 unk_1AC;
    /* 0x01B0 */ s16 unk_1B0;
    /* 0x01B2 */ s16 unk_1B2;
    /* 0x01B4 */ s16 unk_1B4;
    /* 0x01B6 */ s16 unk_1B6;
    /* 0x01B8 */ Vec3f unk_1B8[6];
    /* 0x0200 */ ColliderCylinder unk_200;
    /* 0x0258 */ void* unk_258;
} EnAnubiceFire; // size = 0x25C

extern ActorInit En_Anubice_Fire_InitVars;

#endif
