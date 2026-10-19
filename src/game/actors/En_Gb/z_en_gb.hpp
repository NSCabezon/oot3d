#ifndef _Z_EN_GB_H_
#define _Z_EN_GB_H_

#include "global.hpp"

struct EnGb;

typedef void (*EnGbActionFunc)(struct EnGb*, PlayState*);

typedef struct EnGbCagedSoul {
    /* 0x00 */ u8    infoIdx;
    /* 0x01 */ u8    unk_1;
    /* 0x02 */ u8    unk_2;
    /* 0x03 */ u8    unk_3;
    /* 0x04 */ u8    rotate180;
    /* 0x06 */ s16   unk_6;
    /* 0x08 */ Vec3f translation;
    /* 0x14 */ Vec3f unk_14;
    /* 0x20 */ f32   unk_20;
    /* 0x24 */ f32   unk_24;
    /* 0x28 */ f32   unk_28;
} EnGbCagedSoul; // size = 0x2C

typedef struct EnGb {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ char unk_240[0x1C0];
    /* 0x0400 */ char unk_400[0x250];
    /* 0x0650 */ EnGbActionFunc actionFunc;
    /* 0x0654 */ ColliderCylinder collider;
    /* 0x06AC */ ColliderCylinder bottlesColliders[3];
    /* 0x07B4 */ void* light;
    /* 0x07B8 */ char unk_7B8[0x18];
    /* 0x07D0 */ u16 textId;
    /* 0x07D2 */ char unk_7D2[0x2];
    /* 0x07D4 */ s16 actionTimer;
    /* 0x07D6 */ char unk_7D6[0x6];
    /* 0x07DC */ EnGbCagedSoul cagedSouls[4];
    /* 0x088C */ SkeletonAnimationModel* skelAnimModel[5];
} EnGb; // size = 0x8A0

extern ActorInit En_Gb_InitVars;

#endif
