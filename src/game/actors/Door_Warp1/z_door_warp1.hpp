#ifndef _Z_DOOR_WARP1_H_
#define _Z_DOOR_WARP1_H_

#include "global.hpp"

struct DoorWarp1;

typedef void (*DoorWarp1ActionFunc)(struct DoorWarp1*, PlayState*);

typedef struct DoorWarp1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1BC];
    /* 0x0360 */ char skelCurve[0x8];
    /* 0x0368 */ ZARInfo* zarInfo;
    /* 0x036C */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0370 */ char unk_370[0x38];
    /* 0x03A8 */ char unk_3A8[0x90];
    /* 0x0438 */ f32 unk_438;
    /* 0x043C */ void* cmabs[9];
    /* 0x0460 */ u8 unk_460;
    /* 0x0461 */ char unk_461[0x1B];
    /* 0x047C */ f32 scale;
    /* 0x0480 */ char unk_480[0x4];
    /* 0x0484 */ f32 unk_484;
    /* 0x0488 */ s16 unk_488;
    /* 0x048A */ char unk_48A[0x6];
    /* 0x0490 */ DoorWarp1ActionFunc actionFunc;
    /* 0x0494 */ void* upperLight;
    /* 0x0498 */ char upperLightInfo[0x18];
    /* 0x04B0 */ void* lowerLight;
    /* 0x04B4 */ char lowerLightInfo[0x1C];
} DoorWarp1; // size = 0x4D0

extern ActorInit Door_Warp1_InitVars;

#endif
