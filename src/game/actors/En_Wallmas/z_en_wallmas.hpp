#ifndef _Z_EN_WALLMAS_H_
#define _Z_EN_WALLMAS_H_

#include "global.hpp"

struct EnWallmas;

typedef void (*EnWallmasActionFunc)(struct EnWallmas*, struct PlayState*);

typedef struct EnWallmas {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x5B0];
    /* 0x07D8 */ struct SkeletonAnimationModel* skelAnimModel;
    /* 0x07DC */ EnWallmasActionFunc actionFunc;
    /* 0x07E0 */ s16 timer;
    /* 0x07E2 */ s16 switchFlag;
    /* 0x07E4 */ f32 yTarget;
    /* 0x07E8 */ ColliderCylinder collider;
    /* 0x0840 */ f32 unk_840; // player-relative height offset used by TakePlayer (3DS only)
} EnWallmas; // size = 0x844

extern ActorInit En_Wallmas_InitVars;

#endif
