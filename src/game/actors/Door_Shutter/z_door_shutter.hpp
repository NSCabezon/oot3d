#ifndef _Z_DOOR_SHUTTER_H_
#define _Z_DOOR_SHUTTER_H_

#include "global.hpp"

struct DoorShutter;

typedef void (*DoorShutterActionFunc)(struct DoorShutter*, PlayState*);

typedef struct DoorShutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s16 isActive; // Set to true by player when using the door. Also a timer for niche cases
    /* 0x01BE */ s16 jabuDoorClosedAmount;
    /* 0x01C0 */ s16 bossDoorTexIndex;
    /* 0x01C2 */ u8 doorType;
    /* 0x01C3 */ u8 styleType;
    /* 0x01C4 */ u8 gfxType;
    /* 0x01C5 */ s8 requiredObjectSlot;
    /* 0x01C6 */ u8 unlockTimer;
    /* 0x01C7 */ char unk_1C7[0x1];
    /* 0x01C8 */ s16 actionTimer;
    /* 0x01CA */ char unk_1CA[0x2];
    /* 0x01CC */ f32 barsClosedAmount; // Ranges from 0.0f (unbarred) to 1.0f (barred)
    /* 0x01D0 */ DoorShutterActionFunc actionFunc;
    /* 0x01D4 */ struct SkeletonAnimationModel* skelAnimModel[8];
    /* 0x01F4 */ void* unk_1F4[2]; // freed with FUN_0035021c in Destroy
    /* 0x01FC */ SkeletonAnimationModel_unk_0C* unk_1FC[2]; // freed through gUnk_0055A1C0 in Destroy
    /* 0x0204 */ char unk_204[0x18];
    /* 0x021C */ s32 unk_21C;
} DoorShutter; // size = 0x0220

extern ActorInit Door_Shutter_InitVars;

#endif
