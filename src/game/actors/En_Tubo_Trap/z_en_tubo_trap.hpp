#ifndef _Z_EN_TUBO_TRAP_H_
#define _Z_EN_TUBO_TRAP_H_

#include "global.hpp"

struct EnTuboTrap;

typedef void (*EnTuboTrapActionFunc)(struct EnTuboTrap*, PlayState*);

typedef struct EnTuboTrap {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnTuboTrapActionFunc actionFunc;
    /* 0x01A8 */ union {
        f32 targetY;   // Levitate/Fly target height
        s16 spinSpeed; // WaitForProximity (0x11da78) reads this as a s16 rotation step
    };
    /* 0x01AC */ Vec3f originPos;
    /* 0x01B8 */ ColliderCylinder collider;
    /* 0x0210 */ SkeletonAnimationModel* skelAnimModel;
} EnTuboTrap; // size = 0x214

extern ActorInit En_Tubo_Trap_InitVars;

#endif
