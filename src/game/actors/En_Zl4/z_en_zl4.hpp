#ifndef _Z_EN_ZL4_H_
#define _Z_EN_ZL4_H_

#include "global.hpp"

struct EnZl4;

typedef struct EnZl4 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnim;
    /* 0x03F4 */ char unk_3F4[0x4];
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ char unk_450[0xAE0];
} EnZl4; // size = 0xF30

extern ActorInit En_Zl4_InitVars;

#endif
