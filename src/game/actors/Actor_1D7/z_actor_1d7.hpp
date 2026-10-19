#ifndef _Z_ACTOR_1D7_H_
#define _Z_ACTOR_1D7_H_

#include "global.hpp"

struct Actor1D7;

typedef struct Actor1D7 {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C0 */ u16 timer;
    /* 0x01C2 */ u8 unk_1C2;
    /* 0x01C3 */ u8 hasBgActor;
    /* 0x01C4 */ f32 unk_1C4;
    /* 0x01C8 */ f32 unk_1C8;
    /* 0x01CC */ f32 unk_1CC;
} Actor1D7; // size = 0x1D0

extern ActorInit Actor_1D7_InitVars;

#endif
