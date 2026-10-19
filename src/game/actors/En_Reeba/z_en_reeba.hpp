#ifndef _Z_EN_REEBA_H_
#define _Z_EN_REEBA_H_

#include "global.hpp"

struct EnReeba;

typedef void (*EnReebaActionFunc)(struct EnReeba*, PlayState*);

typedef struct EnReeba {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[18];
    /* 0x0294 */ char unk_294[0xCC];
    /* 0x0360 */ Vec3s morphTable[18];
    /* 0x03CC */ char unk_3CC[0xCC];
    /* 0x0498 */ u8 unk_498;
    /* 0x0499 */ u8 unk_499;
    /* 0x049A */ char unk_49A[0x6];
    /* 0x04A0 */ EnReebaActionFunc actionfunc;
    /* 0x04A4 */ s16 bigLeeverTimer;
    /* 0x04A6 */ s16 moveTimer;
    /* 0x04A8 */ s16 sfxTimer;
    /* 0x04AA */ s16 damagedTimer;
    /* 0x04AC */ s16 waitTimer;
    /* 0x04AE */ u16 type;
    /* 0x04B0 */ s16 unkDamageField;
    /* 0x04B2 */ s16 stunType;
    /* 0x04B4 */ s16 aimType;
    /* 0x04B8 */ f32 yOffsetTarget;
    /* 0x04BC */ f32 yOffsetStep;
    /* 0x04C0 */ f32 scale;
    /* 0x04C4 */ ColliderCylinder collider;
} EnReeba; // size = 0x051C

extern ActorInit En_Reeba_InitVars;

#endif
