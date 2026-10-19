#ifndef _Z_BG_DODOAGO_H_
#define _Z_BG_DODOAGO_H_

#include "global.hpp"

struct BgDodoago;

typedef enum BgDodoagoEye {
    /* 0 */ BGDODOAGO_EYE_LEFT,
    /* 1 */ BGDODOAGO_EYE_RIGHT
} BgDodoagoEye;

typedef void (*BgDodoagoActionFunc)(struct BgDodoago*, PlayState*);

typedef struct BgDodoago {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s16 state; // BgDodoagoEye or a timer-like value
    /* 0x01C0 */ ColliderCylinder mainCollider;  // Used to detect explosions for lighting the eyes
    /* 0x0218 */ ColliderCylinder leftCollider;  // OC-colliding bombs have their xz speed cleared and timer set
    /* 0x0270 */ ColliderCylinder rightCollider; // same as leftCollider
    /* 0x02C8 */ BgDodoagoActionFunc actionFunc;
    /* 0x02CC */ SkeletonAnimationModel* skelAnimModel;
} BgDodoago; // size = 0x2D0

extern ActorInit Bg_Dodoago_InitVars;

#endif
