#ifndef _Z_EN_BOM_H_
#define _Z_EN_BOM_H_

#include "global.hpp"

struct EnBom;

typedef void (*EnBomActionFunc)(struct EnBom*, PlayState*);

typedef struct EnBom {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder bombCollider;
    /* 0x01FC */ char explosionCollider[0x20]; // ColliderJntSph base
    /* 0x021C */ char explosionColliderElements[0x50];
    /* 0x026C */ s16 timer;
    /* 0x026E */ s16 flashSpeedScale;
    /* 0x0270 */ f32 flashIntensity;
    /* 0x0274 */ char unk_274[0x4];
    /* 0x0278 */ SkeletonAnimationModel* model;
    /* 0x027C */ s32 unk_27C;
    /* 0x0280 */ s32 unk_280;
    /* 0x0284 */ u8 unk_284;
    /* 0x0285 */ u8 unk_285;
    /* 0x0286 */ char unk_286[0x2];
    /* 0x0288 */ s32 unk_288;
    /* 0x028C */ s32 unk_28C;
    /* 0x0290 */ EnBomActionFunc actionFunc;
} EnBom; // size = 0x294

extern ActorInit En_Bom_InitVars;

#endif
