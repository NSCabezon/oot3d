#ifndef _Z_MAGIC_DARK_H_
#define _Z_MAGIC_DARK_H_

#include "global.hpp"

struct MagicDark;

typedef struct MagicDark {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 timer;
    /* 0x01A6 */ u8 primAlpha;
    /* 0x01A7 */ u8 pad_1A7;
    /* 0x01A8 */ Vec3f orbOffset;
    /* 0x01B4 */ f32 scale;
    /* 0x01B8 */ char unk_1B8[0x4];
    /* 0x01BC */ SkeletonAnimationModel* model;
    /* 0x01C0 */ void* cmb;
    /* 0x01C4 */ void* freerModel;
    /* 0x01C8 */ void* mgrModel[2];
} MagicDark; // size = 0x1D0

extern ActorInit Magic_Dark_InitVars;

#endif
