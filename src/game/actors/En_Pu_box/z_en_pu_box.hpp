#ifndef _Z_EN_PU_BOX_H_
#define _Z_EN_PU_BOX_H_

#include "global.hpp"

struct EnPubox;

typedef struct EnPubox {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId; // DynaPolyActor layout, spelled out so unk_1B0 can be u16
    /* 0x01A8 */ f32 unk_1A8; // N64 dyna.unk_150: added to speedXZ each update
    /* 0x01AC */ f32 unk_1AC; // N64 dyna.unk_154: cleared each update
    /* 0x01B0 */ u16 unk_1B0; // N64 dyna.unk_158: copied to world.rot.y each update
    /* 0x01B2 */ char unk_1B2[0x2];
    /* 0x01B4 */ u32 interactFlags;
    /* 0x01B8 */ u8 transformFlags;
    /* 0x01B9 */ char unk_1B9[0x3];
    /* 0x01BC */ s32 unused_1BC; // N64 unused_164
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} EnPubox; // size = 0x1C4

extern ActorInit En_Pu_box_InitVars;

#endif
