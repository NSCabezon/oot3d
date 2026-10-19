#ifndef _Z_DOOR_GERUDO_H_
#define _Z_DOOR_GERUDO_H_

#include "global.hpp"

struct DoorGerudo;

typedef void (*DoorGerudoActionFunc)(struct DoorGerudo*, PlayState*);

// Sub-object at 0x1C8, zeroed by FUN_00342968, filled by FUN_00353214, freed by FUN_0034291c
typedef struct DoorGerudoLock {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ void* unk_08[4];
} DoorGerudoLock; // size = 0x18

typedef struct DoorGerudo {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ char unk_1BC[0x2];
    /* 0x01BE */ u8 unk_1BE; // Door lock type drawn when non-zero (N64 unk_166)
    /* 0x01BF */ char unk_1BF[0x1];
    /* 0x01C0 */ DoorGerudoActionFunc actionFunc;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ DoorGerudoLock lock;
} DoorGerudo; // size = 0x1E0

extern ActorInit Door_Gerudo_InitVars;

#endif
