#ifndef _Z_OBJ_HANA_H_
#define _Z_OBJ_HANA_H_

#include "global.hpp"

struct ObjHana;

typedef struct ObjHana {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkeletonAnimationModel* skelAnimModel;
} ObjHana; // size = 0x200

extern ActorInit Obj_Hana_InitVars;

#endif
