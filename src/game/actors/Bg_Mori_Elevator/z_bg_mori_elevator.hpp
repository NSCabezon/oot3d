#ifndef _Z_BG_MORI_ELEVATOR_H_
#define _Z_BG_MORI_ELEVATOR_H_

#include "global.hpp"

struct BgMoriElevator;

typedef void (*BgMoriElevatorActionFunc)(struct BgMoriElevator*, PlayState*);

typedef struct BgMoriElevator {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriElevatorActionFunc actionFunc;
    /* 0x01C0 */ f32 targetY;
    /* 0x01C4 */ s32 unk_16C;
    /* 0x01C8 */ u8 unk_170;
    /* 0x01C9 */ s8 moriTexObjectSlot;
    /* 0x01CA */ s16 unk_172;
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01D0 */ s32 unk_1D0;
    /* 0x01D4 */ s32 unk_1D4;
} BgMoriElevator; // size = 0x1D8

extern ActorInit Bg_Mori_Elevator_InitVars;

#endif
