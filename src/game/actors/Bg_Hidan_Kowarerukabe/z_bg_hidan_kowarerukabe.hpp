#ifndef _Z_BG_HIDAN_KOWARERUKABE_H_
#define _Z_BG_HIDAN_KOWARERUKABE_H_

#include "global.hpp"

struct BgHidanKowarerukabe;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now; same as Bg_Jya_Bombiwa).
typedef struct {
    /* 0x00 */ u8 unk_00[0x2C];
    /* 0x2C */ f32 modelCenterY;
    /* 0x30 */ u8 unk_30[0x4];
    /* 0x34 */ f32 modelRadius;
    /* 0x38 */ u8 unk_38[0x18];
} BgHidanKowarerukabeColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgHidanKowarerukabeColliderElement* elements;
} BgHidanKowarerukabeColliderJntSph; // size = 0x20

typedef struct BgHidanKowarerukabe {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanKowarerukabeColliderJntSph collider;
    /* 0x01DC */ BgHidanKowarerukabeColliderElement colliderElements[1];
    /* 0x022C */ char unk_22C[0x4];
    /* 0x0230 */ SkeletonAnimationModel* skelAnimModel;
} BgHidanKowarerukabe; // size = 0x234

extern ActorInit Bg_Hidan_Kowarerukabe_InitVars;

#endif
