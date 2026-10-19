#ifndef _Z_OBJ_HSBLOCK_H_
#define _Z_OBJ_HSBLOCK_H_

#include "global.hpp"

struct ObjHsblock;

typedef void (*ObjHsblockActionFunc)(struct ObjHsblock*, PlayState*);

typedef struct ObjHsblock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjHsblockActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel2;
} ObjHsblock; // size = 0x01C8

extern ActorInit Obj_Hsblock_InitVars;

#endif
