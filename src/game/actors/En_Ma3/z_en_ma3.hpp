#ifndef _Z_EN_MA3_H_
#define _Z_EN_MA3_H_

#include "global.hpp"

struct EnMa3;

typedef void (*EnMa3ActionFunc)(struct EnMa3*, PlayState*);

typedef struct EnMa3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x84];
    /* 0x0228 */ char unk_228[0x1CC]; // FaceAnimations (accessed through a cast)
    /* 0x03F4 */ EnMa3ActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ s16 unk_450;
    /* 0x0452 */ char unk_452[0x28];
    /* 0x047A */ s16 unk_47A;
    /* 0x047C */ s16 blinkTimer;
    /* 0x047E */ s16 eyeIndex;
    /* 0x0480 */ s16 mouthIndex;
    /* 0x0482 */ char unk_482[0x2];
} EnMa3; // size = 0x484

extern ActorInit En_Ma3_InitVars;

#endif
