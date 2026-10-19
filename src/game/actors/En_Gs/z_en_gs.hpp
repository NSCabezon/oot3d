#ifndef _Z_EN_GS_H_
#define _Z_EN_GS_H_

#include "global.hpp"

struct EnGs;

typedef void (*EnGsActionFunc)(struct EnGs* self, PlayState* play);

typedef struct EnGs {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ EnGsActionFunc actionFunc;
    /* 0x0200 */ u8 unk_19C;
    /* 0x0201 */ u8 unk_19D;
    /* 0x0202 */ u8 unk_19E;
    /* 0x0203 */ u8 unk_19F;
    /* 0x0204 */ Vec3s unk_1A0[3];
    /* 0x0216 */ char unk_216[0x2];
    /* 0x0218 */ Vec3f unk_218[2];
    /* 0x0230 */ char unk_230[0x0C];
    /* 0x023C */ Vec3f unk_23C;
    /* 0x0248 */ Color_RGBA8 flashColor;
    /* 0x024C */ char unk_24C[0x20];
    /* 0x026C */ SkeletonAnimationModel* unk_26C;
    /* 0x0270 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0274 */ SkeletonAnimationModel* unk_274;
    /* 0x0278 */ void* unk_278;
    /* 0x027C */ char unk_27C[0x04];
    /* 0x0280 */ void* unk_280;
} EnGs; // size = 0x284

extern ActorInit En_Gs_InitVars;

#endif
