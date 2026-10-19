#ifndef _Z_OBJ_LIFT_H_
#define _Z_OBJ_LIFT_H_

#include "global.hpp"

struct ObjLift;

typedef void (*ObjLiftActionFunc)(struct ObjLift* self, PlayState* play);

typedef struct ObjLift {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjLiftActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ Vec3s shakeOrientation;
    /* 0x01CA */ s16 timer;
} ObjLift; // size = 0x01CC

extern ActorInit Obj_Lift_InitVars;

#endif
