#ifndef _Z_EN_GE2_H_
#define _Z_EN_GE2_H_

#include "global.hpp"

struct EnGe2;

typedef void (*EnGe2ActionFunc)(struct EnGe2* self, PlayState* play);

typedef struct EnGe2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ char unk_280[0x6E8];
    /* 0x0968 */ s16 eyeIndex;
    /* 0x096A */ s16 blinkTimer;
    /* 0x096C */ Vec3s headRot;
    /* 0x0972 */ Vec3s neckRot; // N64 unk_2EE
    /* 0x0978 */ u16 stateFlags;
    /* 0x097A */ s16 walkDirection;
    /* 0x097C */ s16 yawTowardsPlayer;
    /* 0x097E */ char unk_97E[0x6];
    /* 0x0984 */ u16 walkDuration;
    /* 0x0986 */ u16 walkTimer;
    /* 0x0988 */ char unk_988[0x1];
    /* 0x0989 */ u8 timer;
    /* 0x098A */ u8 playerSpottedParam;
    /* 0x098B */ char unk_98B[0x1];
    /* 0x098C */ EnGe2ActionFunc actionFunc;
    /* 0x0990 */ void* skelAnimModel;
    /* 0x0994 */ FaceAnimations face;
} EnGe2; // size = 0xB60

extern ActorInit En_Ge2_InitVars;

#endif
