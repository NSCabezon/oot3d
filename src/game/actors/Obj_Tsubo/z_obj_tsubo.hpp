#ifndef _Z_OBJ_TSUBO_H_
#define _Z_OBJ_TSUBO_H_

#include "global.hpp"

struct ObjTsubo;

typedef void (*ObjTsuboActionFunc)(struct ObjTsubo*, PlayState*);

typedef struct ObjTsubo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjTsuboActionFunc actionFunc;
    /* 0x01A8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01AC */ ColliderCylinder collider;
    /* 0x0204 */ s8 requiredObjectSlot;
} ObjTsubo; // size = 0x208

extern ActorInit Obj_Tsubo_InitVars;

#endif
