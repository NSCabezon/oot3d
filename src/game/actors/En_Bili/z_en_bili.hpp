#ifndef _Z_EN_BILI_H_
#define _Z_EN_BILI_H_

#include "global.hpp"

struct EnBili;

typedef void (*EnBiliActionFunc)(struct EnBili*, struct PlayState*);

typedef struct EnBili {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x270];
    /* 0x0498 */ char unk_498[0x1CC]; // FaceAnimations (see EnBili_Destroy)
    /* 0x0664 */ EnBiliActionFunc actionFunc;
    /* 0x0668 */ u8 tentaclesTexIndex;
    /* 0x0669 */ u8 playFlySfx;
    /* 0x066A */ s16 timer;
    /* 0x066C */ s16 unk_66C;
    /* 0x066E */ char unk_66E[0x2];
    /* 0x0670 */ ColliderCylinder collider;
} EnBili; // size = 0x6C8

extern ActorInit En_Bili_InitVars;

#endif
