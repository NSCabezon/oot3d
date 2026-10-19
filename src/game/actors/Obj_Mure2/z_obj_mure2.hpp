#ifndef _Z_OBJ_MURE2_H_
#define _Z_OBJ_MURE2_H_

#include "global.hpp"

struct ObjMure2;

typedef void (*ObjMure2ActionFunc)(struct ObjMure2*, PlayState*);

typedef struct ObjMure2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjMure2ActionFunc actionFunc;
    /* 0x01A8 */ Actor* actorSpawnPtrList[12]; // N64 0x150: pointers to all of the actors spawned by the parent
    /* 0x01D8 */ u16 currentActorNum;          // N64 0x180: bitmask of children that were already spawned/killed
    /* 0x01DC */ f32 unk_1DC;                  // N64 0x184: distance scale (1.0 or 4.0 depending on cutscene)
} ObjMure2; // size = 0x1E0

extern ActorInit Obj_Mure2_InitVars;

#endif
