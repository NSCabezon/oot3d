#ifndef _Z_EN_BX_H_
#define _Z_EN_BX_H_

#include "global.hpp"

struct EnBx;

typedef struct EnBx {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1A0];
    /* 0x03C8 */ char unk_3C8[0x98]; // object freed by FUN_003685a0
    /* 0x0460 */ char unk_460[0x24C]; // FaceAnimations (accessed through a cast)
    /* 0x06AC */ ColliderCylinder collider;
    /* 0x0704 */ char unk_704[0x80];
} EnBx; // size = 0x784

extern ActorInit En_Bx_InitVars;

#endif
