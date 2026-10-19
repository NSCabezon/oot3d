#ifndef _Z_EN_DOG_H_
#define _Z_EN_DOG_H_

#include "global.hpp"

struct EnDog;

typedef void (*EnDogActionFunc)(struct EnDog*, PlayState*);

typedef struct EnDog {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x1D4];
    /* 0x03FC */ char morphTable[0x1D4];
    /* 0x05D0 */ u8 unk_5D0; // gates Actor_UpdateBgCheckInfo in Update (set to 1 in Init)
    /* 0x05D1 */ char unk_5D1[0x3];
    /* 0x05D4 */ EnDogActionFunc actionFunc;
    /* 0x05D8 */ ColliderCylinder collider;
    /* 0x0630 */ Path* path;
    /* 0x0634 */ u8 reverse;
    /* 0x0636 */ s16 waypoint;
    /* 0x0638 */ s16 unusedAngle;
    /* 0x063A */ char unk_63A[0x2];
    /* 0x063C */ s16 behaviorTimer;
    /* 0x063E */ char unk_63E[0x2];
    /* 0x0640 */ s16 nextBehavior;
    /* 0x0642 */ s16 behavior;
} EnDog; // size = 0x0644

extern ActorInit En_Dog_InitVars;

#endif
