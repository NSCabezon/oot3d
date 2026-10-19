#ifndef _Z_EN_FIRE_ROCK_H_
#define _Z_EN_FIRE_ROCK_H_

#include "global.hpp"

struct EnFireRock;

typedef void (*EnFireRockActionFunc)(struct EnFireRock*, PlayState*);

typedef struct EnFireRock {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3f angularVelocity;
    /* 0x01B0 */ Vec3f rockRotation;
    /* 0x01BC */ char unk_1BC[0x4];
    /* 0x01C0 */ EnFireRockActionFunc actionFunc;
    /* 0x01C4 */ f32 scale;
    /* 0x01C8 */ Vec3f relativePos;
    /* 0x01D4 */ Vec3f unk_1D4; // set but unused
    /* 0x01E0 */ s16 timer;
    /* 0x01E2 */ s16 timer2;
    /* 0x01E4 */ s16 type;
    /* 0x01E6 */ u8 playerNearby;
    /* 0x01E7 */ u8 flameTimer;
    /* 0x01E8 */ struct EnEncount2* spawner;
    /* 0x01EC */ ColliderCylinder collider;
    /* 0x0244 */ SkeletonAnimationModel* skelAnimModel;
} EnFireRock; // size = 0x248

extern ActorInit En_Fire_Rock_InitVars;

#endif
