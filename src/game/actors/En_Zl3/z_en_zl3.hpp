#ifndef _Z_EN_ZL3_H_
#define _Z_EN_ZL3_H_

#include "global.hpp"

struct EnZl3;

typedef void (*EnZl3ActionFunc)(struct EnZl3*, PlayState*);

typedef struct EnZl3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x3F4 - 0x228]; // FaceAnimations lives here (see EnZl3_Destroy)
    /* 0x03F4 */ char unk_3F4[0x9A4 - 0x3F4];
    /* 0x09A4 */ char unk_9A4[0xF54 - 0x9A4];
    /* 0x0F54 */ s32 zarIndex;
    /* 0x0F58 */ char unk_F58[0xF60 - 0xF58];
    /* 0x0F60 */ s32 action;
    /* 0x0F64 */ s32 drawConfig;
    /* 0x0F68 */ char unk_F68[0xF6C - 0xF68];
    /* 0x0F6C */ f32 unk_F6C;
    /* 0x0F70 */ char unk_F70[0x108C - 0xF70];
    /* 0x108C */ ColliderCylinder collider;
    /* 0x108C + sizeof(ColliderCylinder) */ char unk_end[0x1140 - 0x108C - sizeof(ColliderCylinder)];
} EnZl3; // size = 0x1140

extern ActorInit En_Zl3_InitVars;

#endif
