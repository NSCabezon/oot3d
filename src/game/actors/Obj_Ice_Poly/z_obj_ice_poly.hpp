#ifndef _Z_OBJ_ICE_POLY_H_
#define _Z_OBJ_ICE_POLY_H_

#include "global.hpp"

struct ObjIcePoly;

typedef void (*ObjIcePolyActionFunc)(struct ObjIcePoly*, PlayState*);

typedef struct ObjIcePoly {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjIcePolyActionFunc actionFunc;
    /* 0x01A8 */ u8 alpha;
    /* 0x01A9 */ u8 unk_1A9; // Unused. Probably intended to be a switch flag.
    /* 0x01AA */ s16 meltTimer;
    /* 0x01AC */ ColliderCylinder iceCollider;
    /* 0x0204 */ ColliderCylinder hardCollider;
    /* 0x025C */ SkeletonAnimationModel* skelAnimModel;
} ObjIcePoly; // size = 0x260

extern ActorInit Obj_Ice_Poly_InitVars;

#endif
