#ifndef _Z_DOOR_ANA_H_
#define _Z_DOOR_ANA_H_

#include "global.hpp"

struct DoorAna;

typedef void (*DoorAnaActionFunc)(struct DoorAna*, PlayState*);

typedef struct DoorAna {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ DoorAnaActionFunc actionFunc;
    /* 0x0200 */ struct SkeletonAnimationModel* model;
    /* 0x0204 */ s32 switchFlag;
} DoorAna; // size = 0x208

extern ActorInit Door_Ana_InitVars;

#endif
