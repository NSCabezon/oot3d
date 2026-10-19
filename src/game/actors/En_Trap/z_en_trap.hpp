#ifndef _Z_EN_TRAP_H_
#define _Z_EN_TRAP_H_

#include "global.hpp"

struct EnTrap;

typedef struct EnTrap {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 playerDmgTimer;
    /* 0x01A8 */ s16 upperParams;
    /* 0x01AA */ s16 genericVar1;
    /* 0x01AC */ f32 genericVar2;
    /* 0x01B0 */ Vec3f targetPosLeft;
    /* 0x01BC */ Vec3f targetPosRight;
    /* 0x01C8 */ Vec3f targetPosFwd;
    /* 0x01D4 */ Vec3f targetPosBack;
    /* 0x01E0 */ Vec3f moveSpeedLeftRight;
    /* 0x01EC */ Vec3f moveSpeedForwardBack;
    /* 0x01F8 */ ColliderCylinder collider;
    /* 0x0250 */ SkeletonAnimationModel* model;
} EnTrap; // size = 0x254

extern ActorInit En_Trap_InitVars;

#endif
