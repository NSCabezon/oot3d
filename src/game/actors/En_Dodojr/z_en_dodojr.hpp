#ifndef _Z_EN_DODOJR_H_
#define _Z_EN_DODOJR_H_

#include "global.hpp"

struct EnDodojr;

typedef void (*EnDodojrActionFunc)(struct EnDodojr*, PlayState*);

typedef struct EnDodojr {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x138];
    /* 0x0360 */ char morphTable[0x138];
    /* 0x0498 */ EnDodojrActionFunc actionFunc;
    /* 0x049C */ ColliderCylinder collider;
    /* 0x04F4 */ Actor* bomb;
    /* 0x04F8 */ Vec3f headPos;
    /* 0x0504 */ Vec3f dustPos;
    /* 0x0510 */ s16 counter; // Used for bouncing and flashing when dying.
    /* 0x0512 */ s16 stunTimer;
    /* 0x0514 */ s16 freezeFrameTimer;
    /* 0x0516 */ s16 timer; // Used for burrowing/despawning, bomb swallowing, and dying.
    /* 0x0518 */ s16 crawlSfxTimer;
    /* 0x051C */ f32 rootScale; // scale used with the root limb
} EnDodojr; // size = 0x0520

extern ActorInit En_Dodojr_InitVars;

#endif
