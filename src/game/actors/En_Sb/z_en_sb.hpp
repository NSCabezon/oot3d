#ifndef _Z_EN_SB_H_
#define _Z_EN_SB_H_

#include "global.hpp"

struct EnSb;

typedef void (*EnSbActionFunc)(struct EnSb*, PlayState*);

typedef struct EnSb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ EnSbActionFunc actionFunc;
    /* 0x022C */ ColliderCylinder collider;
    /* 0x0284 */ char bodyBreak[0x14];
    /* 0x0298 */ s16 fire;
    /* 0x029A */ s16 behavior;
    /* 0x029C */ s16 isDead;
    /* 0x029E */ s16 timer;
    /* 0x02A0 */ s16 attackYaw;
    /* 0x02A2 */ s16 bouncesLeft; // amount of bounces left in the attack before going back to wait
    /* 0x02A4 */ u8 hitByWindArrow;
} EnSb; // size = 0x02A8

extern ActorInit En_Sb_InitVars;

#endif
