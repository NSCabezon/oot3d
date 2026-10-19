#ifndef _Z_OBJ_ELEVATOR_H_
#define _Z_OBJ_ELEVATOR_H_

#include "global.hpp"

struct ObjElevator;

typedef struct ObjElevator {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ void (*actionFunc)(struct ObjElevator*, PlayState*);
    /* 0x01C0 */ f32 unk_1C0; // target y
    /* 0x01C4 */ f32 unk_1C4; // move speed (from params)
    /* 0x01C8 */ u8 unk_1C8; // previous frame's dyna.unk_1B8 (interact flags)
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel;
} ObjElevator; // size = 0x1D0

extern ActorInit Obj_Elevator_InitVars;

#endif
