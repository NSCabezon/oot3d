#ifndef _Z_EN_STH_H_
#define _Z_EN_STH_H_

#include "global.hpp"

struct EnSth;

typedef void (*EnSthActionFunc)(struct EnSth*, PlayState*);

typedef struct EnSth {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ u16 eventFlag;
    /* 0x0282 */ char unk_282[0x2];
    /* 0x0284 */ ActorFunc drawFunc;
    /* 0x0288 */ u8 requiredObjectSlot;
    /* 0x0289 */ char unk_289[0xD];
    /* 0x0296 */ u16 unk_296;
    /* 0x0298 */ s16 unk_298;
    /* 0x029A */ s16 unk_29A;
    /* 0x029C */ EnSthActionFunc actionFunc;
    /* 0x02A0 */ SkeletonAnimationModel* skelAnimModel;
} EnSth; // size = 0x2A4

extern ActorInit En_Sth_InitVars;

#endif
