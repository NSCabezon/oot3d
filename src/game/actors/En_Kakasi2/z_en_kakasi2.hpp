#ifndef _Z_EN_KAKASI2_H_
#define _Z_EN_KAKASI2_H_

#include "global.hpp"

struct EnKakasi2;

typedef void (*EnKakasi2ActionFunc)(struct EnKakasi2*, PlayState*);

typedef struct EnKakasi2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnKakasi2ActionFunc actionFunc;
    /* 0x01A8 */ SkelAnime skelAnime;
    /* 0x022C */ s16 unk_22C;
    /* 0x022E */ s16 switchFlag;
    /* 0x0230 */ s16 unk_230;
    /* 0x0234 */ Vec3f maxSpawnDistance;
    /* 0x0240 */ f32 height;
    /* 0x0244 */ ColliderCylinder collider;
    /* 0x029C */ s32 unk_29C;
} EnKakasi2; // size = 0x2A0

extern ActorInit En_Kakasi2_InitVars;

#endif
