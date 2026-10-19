#ifndef _Z_ITEM_SHIELD_H_
#define _Z_ITEM_SHIELD_H_

#include "global.hpp"

struct ItemShield;

typedef void (*ItemShieldActionFunc)(struct ItemShield* self, PlayState* play);

typedef struct ItemShield {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ s16 unk_198;
    /* 0x01FE */ s16 timer;
    /* 0x0200 */ u16 unk_19C; // bit 1: hide model
    /* 0x0202 */ u8 unk_19E[8];
    /* 0x020A */ char unk_20A[0x2];
    /* 0x020C */ Vec3f unk_1A8[8];
    /* 0x026C */ ItemShieldActionFunc actionFunc;
    /* 0x0270 */ SkeletonAnimationModel* model;
} ItemShield; // size = 0x274

extern ActorInit Item_Shield_InitVars;

#endif
