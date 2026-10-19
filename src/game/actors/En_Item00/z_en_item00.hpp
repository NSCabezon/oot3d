#ifndef _Z_EN_ITEM00_H_
#define _Z_EN_ITEM00_H_

#include "global.hpp"

struct EnItem00;

typedef struct EnItem00 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void (*actionFunc)(struct EnItem00*, PlayState*);
    /* 0x01A8 */ s16 collectibleFlag;
    /* 0x01AA */ s16 getItemId;
    /* 0x01AC */ s16 unk_1AC;
    /* 0x01AE */ s16 unk_1AE;
    /* 0x01B0 */ s16 unk_1B0;
    /* 0x01B2 */ s16 despawnTimer;
    /* 0x01B4 */ f32 scale;
    /* 0x01B8 */ ColliderCylinder collider;
    /* 0x0210 */ SkeletonAnimationModel* model;
    /* 0x0214 */ u8 unk_214;
    /* 0x0215 */ char unk_215[0x03];
} EnItem00; // size = 0x218

extern ActorInit En_Item00_InitVars;

#endif
