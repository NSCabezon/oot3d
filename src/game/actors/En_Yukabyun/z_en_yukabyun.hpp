#ifndef _Z_EN_YUKABYUN_H_
#define _Z_EN_YUKABYUN_H_

#include "global.hpp"

struct EnYukabyun;

typedef void (*EnYukabyunActionFunc)(struct EnYukabyun*, PlayState*);

typedef struct EnYukabyun {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnYukabyunActionFunc actionFunc;
    /* 0x01A8 */ u16 timer;
    /* 0x01AA */ u8 modelIdx;
    /* 0x01AC */ ColliderCylinder collider;
    /* 0x0204 */ SkeletonAnimationModel* skelAnimModel;
} EnYukabyun; // size = 0x208

extern ActorInit En_Yukabyun_InitVars;

#endif
